#include "Engine.hpp"

struct PushConstants {
    glm::mat4 model{1};
    glm::mat4 view{1};
    glm::mat4 proj{1};
    int subdivision = 0;
};

int main() {
    try {
        Log::init();
        Window::init(1280, 720);
        Context::init();

        Swapchain swapchain{};

        std::vector<glm::vec4> vertices{{1, -1, 0, 1}, {1, 1, 0, 1}, {-1, 1, 0, 1}, {-1, -1, 0, 1}};
        std::vector<uint32_t> indices{0, 1, 2, 3};
        DeviceBuffer vertexBuffer{BufferUsage::Storage, vertices};
        DeviceBuffer indexBuffer{BufferUsage::Storage, indices};

        Camera camera{Window::getWidth(), Window::getHeight()};

        Shader meshShader{SHADER_DIR + "subdivision.mesh"};
        Shader fragShader{SHADER_DIR + "subdivision.frag"};

        DescriptorSet descSet;
        descSet.addResources(meshShader);
        descSet.addResources(fragShader);
        descSet.record("Vertices", vertexBuffer);
        descSet.record("Indices", indexBuffer);
        descSet.allocate();

        GraphicsPipeline pipeline{descSet};
        pipeline.addShader(meshShader);
        pipeline.addShader(fragShader);
        pipeline.setup(swapchain, sizeof(PushConstants));

        int frame = 0;
        PushConstants constants;
        while (!Window::shouldClose()) {
            Window::pollEvents();
            camera.processInput();

            glm::mat4 rotation =
                glm::rotate(glm::mat4(1.0f), 0.01f * frame, glm::vec3(0.0f, 1.0f, 0.0f));
            constants.proj = camera.getProj();
            constants.view = camera.getView();

            swapchain.waitNextFrame();

            CommandBuffer commandBuffer = swapchain.beginCommandBuffer();
            commandBuffer.bindPipeline(pipeline);
            commandBuffer.clearBackImage({0.2f, 0.2f, 0.2f, 1.0f});
            commandBuffer.beginRenderPass();

            // Render default quad (triangulated)
            constants.subdivision = 0;
            constants.model = glm::translate(glm::vec3(-1.5, 0, 0)) * rotation;
            commandBuffer.pushConstants(pipeline, &constants);
            commandBuffer.drawMeshTasks(1, 1, 1);

            // Render subdivided quad (triangulated)
            constants.subdivision = 1;
            constants.model = glm::translate(glm::vec3(1.5, 0, 0)) * rotation;
            commandBuffer.pushConstants(pipeline, &constants);
            commandBuffer.drawMeshTasks(1, 4, 1);  // 4 = subdiv count

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
