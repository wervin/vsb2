#include "vsb2/graphics/pipeline.h"
#include "vsb2/graphics/pipelinelayout.h"
#include "vsb2/graphics/shader.h"
#include "vsb2/graphics/window.h"
#include "vsb2/graphics/device.h"
#include "vsb2/graphics/renderpass.h"

#include "vsb2/log.h"

enum vsb2_error vsb2_graphics_pipeline_init(struct vsb2_graphics_pipeline *pipeline,
                                            struct vsb2_graphics_device *device,
                                            struct vsb2_graphics_renderpass *renderpass,
                                            struct vsb2_graphics_pipelinelayout *pipelinelayout,
                                            struct vsb2_graphics_shader *vertex_shader,
                                            struct vsb2_graphics_shader *frag_shader)
{
    VkPipelineShaderStageCreateInfo shader_stage_info[2] = {0};
    shader_stage_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stage_info[0].module = vertex_shader->vk_shader_module;
    shader_stage_info[0].pName = "main";

    shader_stage_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stage_info[1].module = frag_shader->vk_shader_module;
    shader_stage_info[1].pName = "main";

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {0};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexAttributeDescriptionCount = 0;
    vertex_input_info.vertexBindingDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {0};
    input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_info.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportstate_info = {0};
    viewportstate_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportstate_info.viewportCount = 1;
    viewportstate_info.pViewports = NULL;
    viewportstate_info.scissorCount = 1;
    viewportstate_info.pScissors = NULL;

    // VkViewport viewport = {0};
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = (float) window->width;
    // viewport.height = (float) window->height;
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;

    // VkRect2D scissor = {0};
    // scissor.offset = (VkOffset2D) { .x=0, .y=0 };
    // scissor.extent = (VkExtent2D) { .height=window->height, .width=window->width };

    VkPipelineRasterizationStateCreateInfo rasterization_info = {0};
    rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_info.depthClampEnable = VK_FALSE;
    rasterization_info.rasterizerDiscardEnable = VK_FALSE;
    rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_info.lineWidth = 1.0f;
    rasterization_info.cullMode = VK_CULL_MODE_NONE;
    rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterization_info.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling_info = {0};
    multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_info.sampleShadingEnable = VK_FALSE;
    multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorblend_attachment_info = {0};
    colorblend_attachment_info.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorblend_attachment_info.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorblending_info= {0};
    colorblending_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorblending_info.logicOpEnable = VK_FALSE;
    colorblending_info.logicOp = VK_LOGIC_OP_COPY;
    colorblending_info.attachmentCount = 1;
    colorblending_info.pAttachments = &colorblend_attachment_info;
    colorblending_info.blendConstants[0] = 0.0f;
    colorblending_info.blendConstants[1] = 0.0f;
    colorblending_info.blendConstants[2] = 0.0f;
    colorblending_info.blendConstants[3] = 0.0f;

    VkDynamicState dynamic_states[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicstate_info = {0};
    dynamicstate_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicstate_info.dynamicStateCount = 2;
    dynamicstate_info.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo pipeline_info = {0};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stage_info;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly_info;
    pipeline_info.pViewportState = &viewportstate_info;
    pipeline_info.pRasterizationState = &rasterization_info;
    pipeline_info.pMultisampleState = &multisampling_info;
    pipeline_info.pColorBlendState = &colorblending_info;
    pipeline_info.pDynamicState = &dynamicstate_info;
    pipeline_info.layout = pipelinelayout->vk_pipelinelayout;
    pipeline_info.renderPass = renderpass->vk_renderpass;
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(device->vk_device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &pipeline->vk_pipeline) != VK_SUCCESS)
    {
        VSB2_LOG_ERROR(VSB2_ERROR_VK, "Failed to create pipeline");
        return VSB2_ERROR_VK;
    }

    return VSB2_ERROR_NONE;
}

void vsb2_graphics_pipeline_destroy(struct vsb2_graphics_pipeline *pipeline, struct vsb2_graphics_device *device)
{
    vkDestroyPipeline(device->vk_device, pipeline->vk_pipeline, NULL);
}