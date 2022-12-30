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
        Window::init(750, 750);
        Context::init();

        Swapchain swapchain {};
        std::vector<Vertex> vertices { { { -1, 0, 0 } }, { { 0, -1, 0 } }, { { 1, 0, 0 } } };
        std::vector<Index> indices { 0, 1, 2 };
        Mesh mesh { vertices, indices };
        Camera camera { Window::getWidth(), Window::getHeight() };

        Shader taskShader { "../shader/meshshader.task" };
        Shader meshShader { "../shader/meshshader.mesh" };
        Shader fragShader { "../shader/meshshader.frag" };

        DescriptorSet descSet;
        descSet.addResources(taskShader);
        descSet.addResources(meshShader);
        descSet.addResources(fragShader);
        descSet.record("Vertices", mesh.getVertexBuffer());
        descSet.record("Indices", mesh.getIndexBuffer());
        descSet.allocate();

        GraphicsPipeline pipeline { descSet };
        pipeline.addShader(taskShader);
        pipeline.addShader(meshShader);
        pipeline.addShader(fragShader);
        pipeline.setup(swapchain, sizeof(PushConstants));

        int frame = 0;
        while (!Window::shouldClose()) {
            Window::pollEvents();
            camera.processInput();

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
            commandBuffer.drawMeshTasks(1, 1, 1);
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
