//
// Created by camila on 11/10/25.
//

#include "FrameGraphViz.hpp"

#include "GLTexture.hpp"
void FrameGraphViz::OnStart() {
  struct TriangleTest {
    FG::Resource colorImage;
  };
  bb.add<TriangleTest>() = fg.push_pass_lambda<TriangleTest>(
      "TriangleTest",
      [](FG::FrameGraph::Builder &builder, TriangleTest &data) {
        //this pass creates a transient image
        data.colorImage = builder.create<GLTexture>("ColorImage",GLTexture::Desc{.size = {100,100}});

        //this pass writes to it
        data.colorImage = builder.write(data.colorImage);
      },
      [](const TriangleTest &data, FG::FrameGraph::PassResources& resources) {
        //at rendertime. get the actual resource handle
        const GLTexture& texture = resources.get<GLTexture>(data.colorImage);
        
      });

  /*
  bb.add<TriangleTest>() = fg.add_callback_pass<TriangleTest>("TriangleTest",
                                                              [](FrameGraph::Builder
  &builder, TriangleTest &data) { data.colorImage =
  builder.create<GLTexture>("ColorImage",GLTexture::Desc{.size = {100,100}});
                                                                  data.colorImage
  = builder.write(data.colorImage);
                                                              },
                                                              [](const
  TriangleTest &data, FrameGraphPassResources &resources, void *ctx) {
                                                                  resources.get<GLTexture>(data.colorImage);
                                                              });*/
  fg.compile();
  fg.execute();
}

void FrameGraphViz::OnUpdate() {}

void FrameGraphViz::OnRender() {
  ImGui::ShowDemoWindow();
  fg.execute();
}

void FrameGraphViz::OnShutdown() {}
