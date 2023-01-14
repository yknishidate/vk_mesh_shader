#include "Engine.hpp"

struct Constants {
    glm::mat4 model{1};
    glm::mat4 view{1};
    glm::mat4 proj{1};
};

int main() {
    try {
        Log::init();
        Window::init(1280, 720);
        Context::init();

        Swapchain swapchain{};

        Mesh bunny{"bunny_and_teapot.obj"};
        Object bunnyInstance{bunny};
        TopAccel topAccel{bunnyInstance};

        Camera camera{Window::getWidth(), Window::getHeight()};
        camera.setPosition(0, -5, 15);

        Shader meshShader{SHADER_DIR + "mesh_ray_query.mesh"};
        Shader fragShader{SHADER_DIR + "mesh_ray_query.frag"};

        DescriptorSet descSet;
        descSet.addResources(meshShader);
        descSet.addResources(fragShader);
        descSet.record("Vertices", bunny.getVertexBuffer());
        descSet.record("Indices", bunny.getIndexBuffer());
        descSet.record("topLevelAS", topAccel);
        descSet.allocate();

        GraphicsPipeline pipeline{descSet};
        pipeline.addShader(meshShader);
        pipeline.addShader(fragShader);
        pipeline.setup(swapchain, sizeof(Constants));

        int frame = 0;
        Constants constants;
        while (!Window::shouldClose()) {
            Window::pollEvents();
            camera.processInput();

            constants.proj = camera.getProj();
            constants.view = camera.getView();

            swapchain.waitNextFrame();

            CommandBuffer commandBuffer = swapchain.beginCommandBuffer();
            commandBuffer.bindPipeline(pipeline);
            commandBuffer.clearBackImage({0.0f, 0.0f, 0.2f, 1.0f});
            commandBuffer.beginRenderPass();
            commandBuffer.pushConstants(pipeline, &constants);
            commandBuffer.drawMeshTasks(bunny.getTriangleCount(), 1, 1);

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
