#include "Engine.hpp"

struct PushConstants {
    glm::mat4 model { 1 };
    glm::mat4 view { 1 };
    glm::mat4 proj { 1 };
};

int main()
{
    try {
        Log::init();
        Window::init(1280, 720);
        Context::init();

        Swapchain swapchain {};
        GUI gui { swapchain };

        std::vector<Vertex> vertices {
            { { 1, -1, 0 } },
            { { 1, 1, 0 } },
            { { -1, 1, 0 } },
            { { -1, -1, 0 } }
        };
        std::vector<Index> indices { 0, 1, 3, 1, 2, 3 };
        Mesh mesh { vertices, indices };

        Camera camera { Window::getWidth(), Window::getHeight() };
        camera.setPosition(0.0f, 0.0f, 20.0f);

        Shader meshShader { SHADER_DIR + "meshshader.mesh" };
        Shader fragShader { SHADER_DIR + "meshshader.frag" };

        DescriptorSet descSet;
        descSet.addResources(meshShader);
        descSet.addResources(fragShader);
        descSet.record("Vertices", mesh.getVertexBuffer());
        descSet.record("Indices", mesh.getIndexBuffer());
        descSet.allocate();

        GraphicsPipeline pipeline { descSet };
        pipeline.addShader(meshShader);
        pipeline.addShader(fragShader);
        pipeline.setup(swapchain, sizeof(PushConstants));

        int frame = 0;
        int instancesCount = 3;
        while (!Window::shouldClose()) {
            Window::pollEvents();
            camera.processInput();

            gui.startFrame();
            gui.sliderInt("Instances", instancesCount, 1, 10);

            PushConstants pushConstants;
            pushConstants.model = glm::rotate(glm::mat4(1.0f), 0.01f * frame, glm::vec3(0.0f, 1.0f, 0.0f));
            pushConstants.proj = camera.getProj();
            pushConstants.view = camera.getView();

            swapchain.waitNextFrame();

            CommandBuffer commandBuffer = swapchain.beginCommandBuffer();
            commandBuffer.bindPipeline(pipeline);
            commandBuffer.pushConstants(pipeline, &pushConstants);
            commandBuffer.clearBackImage({ 0.0f, 0.0f, 0.3f, 1.0f });
            commandBuffer.beginRenderPass();
            commandBuffer.drawMeshTasks(mesh.getTriangleCount(), instancesCount, 1);
            commandBuffer.drawGUI(gui);
            commandBuffer.endRenderPass();
            commandBuffer.submit();

            swapchain.present();
            frame++;
        }
        Context::waitIdle();
        Window::shutdown();
    } catch (const std::exception& e) {
        Log::error(e.what());
    }
}
