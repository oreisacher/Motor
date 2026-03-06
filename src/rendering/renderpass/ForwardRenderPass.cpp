//
// Created by Orik on 23/10/2025.
//

#include <motor/rendering/renderpass/ForwardRenderPass.h>
#include <motor/rendering/OpenGLRenderer.h>
#include <motor/rendering/WindowManager.h>
#include <motor/utils/ShaderUniformHelper.h>
#include <motor/assets/AssetManager.h>
#include <motor/assets/ModelInstance.h>

#include <format>

using namespace Motor;

ForwardRenderPass::ForwardRenderPass() {
    skybox = AssetManager::getInstance()->load<Material>("motor://materials/skybox.material");
}

void ForwardRenderPass::execute(const FrameRenderData &frameData, RenderPassResourceRegistry &registry, GPUResourceManager &resourceManager) {
    glBindFramebuffer(GL_FRAMEBUFFER, registry.get("sceneFBO"));

    for (auto &[key, command] : frameData.batchedCommands) {
        GPUShader* gpuShader = resourceManager.loadShader(command.material->getShader());
        glUseProgram(gpuShader->id);

        GPUMesh* gpuMesh = resourceManager.loadMesh(command.mesh);

        // --- Set Standard Uniforms ---
        setUniformMat4(gpuShader->id, "view", glm::inverse(frameData.cameras[frameData.activeCamera]->getGlobalTransform()));
        setUniformMat4(gpuShader->id, "projection", frameData.cameras[frameData.activeCamera]->getProjectionMatrix());
        setUniformVec3(gpuShader->id, "viewPos", frameData.cameras[frameData.activeCamera]->getPosition());

        // --- Set Material Uniforms ---
        setUniformVec3(gpuShader->id, "baseColor", command.material->getColor());
        setUniformFloat(gpuShader->id, "metallic", command.material->getMetallic());
        setUniformFloat(gpuShader->id, "roughness", command.material->getRoughness());
        setUniformFloat(gpuShader->id, "tiling", command.material->getTextureTiling());

        // --- Set Additional Uniforms ---
        setUniformBool(gpuShader->id, "instanced", true);

        // --- Set Texture Uniforms ---
        setUniformBool(gpuShader->id, "hasDiffuse", command.material->hasDiffuseTexture());
        setUniformBool(gpuShader->id, "hasMetallic", command.material->hasMetallicTexture());
        setUniformBool(gpuShader->id, "hasRoughness", command.material->hasRoughnessTexture());
        setUniformBool(gpuShader->id, "hasAO", command.material->hasAOTexture());
        setUniformBool(gpuShader->id, "hasNormal", command.material->hasNormalTexture());

        int textureSlot = 0;
        for (auto& sampler : command.material->getTextureNames()) {
            GPUTexture* gpuTexture = resourceManager.loadTexture(command.material->getTexture(sampler));

            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(GL_TEXTURE_2D, gpuTexture->id);
            setUniformInt(gpuShader->id, sampler, textureSlot);
            textureSlot++;
        }

        // --- Set Lighting Uniforms ---
        setUniformInt(gpuShader->id, "numLights", static_cast<int>(frameData.lights.size()));
        setUniformFloat(gpuShader->id, "shadowMapResolution", registry.get("shadowMapResolution"));

        for (int i = 0 ; i < frameData.lights.size() ; i++) {
            std::string baseName = std::format("lights[{}].", i);
            setUniformInt   (gpuShader->id, baseName + "type", static_cast<int>(frameData.lights[i]->getLightType()));
            setUniformVec3  (gpuShader->id, baseName + "color", frameData.lights[i]->getColor());
            setUniformFloat (gpuShader->id, baseName + "intensity", frameData.lights[i]->getIntensity());
            setUniformVec3  (gpuShader->id, baseName + "position", frameData.lights[i]->getGlobalPosition());
            setUniformVec3  (gpuShader->id, baseName + "direction", frameData.lights[i]->getDirection());
            setUniformFloat (gpuShader->id, baseName + "linear", frameData.lights[i]->getLinear());
            setUniformFloat (gpuShader->id, baseName + "quadratic", frameData.lights[i]->getQuadratic());
            setUniformFloat (gpuShader->id, baseName + "innerAngle", glm::cos(glm::radians(frameData.lights[i]->getInnerAngle())));
            setUniformFloat (gpuShader->id, baseName + "outerAngle", glm::cos(glm::radians(frameData.lights[i]->getOuterAngle())));

            // --- Shadow Map ---
            GLuint shadowMapTexture = registry.get(std::format("shadowMap{}", i));

            if (shadowMapTexture != 0) {
                glActiveTexture(GL_TEXTURE0 + textureSlot);
                glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
                setUniformInt(gpuShader->id, baseName + "shadowMap", textureSlot);
                setUniformMat4(gpuShader->id, baseName + "view", glm::inverse(frameData.lights[i]->getGlobalTransform()));
                setUniformMat4(gpuShader->id, baseName + "projection", frameData.lights[i]->getProjectionMatrix());
                textureSlot++;
            }
        }

        // --- Set Skybox Texture ---
        setUniformVec3(gpuShader->id, "skyboxColor", skybox->getColor());
        setUniformBool(gpuShader->id, "hasSkybox", skybox->getTexture("skybox") != nullptr);

        if (skybox->getTexture("skybox")) {
            GPUTexture* gpuTexture = resourceManager.loadTexture(skybox->getTexture("skybox"));
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(GL_TEXTURE_2D, gpuTexture->id);
            setUniformInt(gpuShader->id, "skybox", textureSlot);
            textureSlot++;
        }

        // --- Set Skinning Uniforms ---
        auto it = frameData.meshModelInstanceLookup.find(command.mesh);
        if (it != frameData.meshModelInstanceLookup.end()) {
            setUniformBool(gpuShader->id, "skinned", it->second->getModel()->isSkinned());

            for (int boneIndex = 0 ; boneIndex < it->second->getModel()->getBoneTransforms().size() ; boneIndex++) {
                setUniformMat4(gpuShader->id, "u_jointMat[" + std::to_string(boneIndex) + "]", it->second->boneTransforms[boneIndex]);
            }
        }

        // resourceManager.updateInstanceBuffer(gpuMesh, command.modelTransforms);

        // --- Draw Call ---
        glBindVertexArray(gpuMesh->vao);
        glDrawElementsInstanced(GL_TRIANGLES, command.mesh->getIndices().size(), GL_UNSIGNED_INT, 0, command.modelTransforms.size());
    }

    glBindVertexArray(0);
}

void ForwardRenderPass::updateSize(int width, int height) { }
