#include "AtlasDebugViewport.hpp"

static vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b * cos(6.283185f * (c * t + d));
}
void AtlasDebugViewport::WindowContents()
{
    // static mat4 ProjMat = glm::ortho(2, 2, 2, 2);
    // float Time = glfwGetTime() * 0.25f;
    // static mat4 ViewMat = glm::lookAt(vec3(glm::sin(Time), 1.5, glm::cos(Time)), vec3(0), vec3(0, 1, 0));

    static int32 AxisView = 0;
    std::pair<uint32, uint32> Axes = {std::max(AxisView - 1, 0), glm::min(AxisView + 1, 2)};
    static float WidthModifier = 1.0f;
    static bool Filled = true;
    static ivec2 RangeView = {0, octree.GetDepth()};
    int NodeCount = 0;
    struct Quads
    {
        ImVec2 Min, Max;
        ImVec4 Color;
        bool Leaf;
        float width = 1.0f;
    };
    static std::map<uint32, std::vector<Quads>> QuadsToDraw;
    QuadsToDraw.clear();
    octree.ForEach([&](const decltype(octree)::NodeData &data) {
        NodeCount++;
        const auto nodeSize = octree.SizeOfNodeAtDepth(data.Depth);
        ImVec2 min = {data.NodePos[Axes.first], data.NodePos[Axes.second]}, max = {min.x + nodeSize, min.y + nodeSize};
        min.x = glm::min(min.x, max.x);
        min.y = glm::min(min.y, max.y);

        max.x = glm::max(min.x, max.x);
        max.y = glm::max(min.y, max.y);
        if (data.Depth < RangeView.x || data.Depth > RangeView.y)
            return;
        float DepthF = data.Depth / (octree.GetDepth() - 1.0f);

        vec3 Color = palette(DepthF, {0.538, 0.538, 0.538}, {0.718, 0.438, 0.720}, {0.520, 0.800, 0.520},
                             {-0.430, -0.397, -0.083});
        if (data.isLeaf)
            Color = vec3(1);
        QuadsToDraw[data.Depth].push_back(Quads{.Min = min,
                                                .Max = max,
                                                .Color = ImVec4(Color.x, Color.y, Color.z, 1.0f),
                                                .Leaf = data.isLeaf,
                                                .width = 1.0f * WidthModifier * (octree.GetDepth() - data.Depth + 1)});
    });

    // ImVec2 PlotPos = ImPlot::GetPlotPos();
    // ImVec2 PlotSize = ImPlot::GetPlotSize();
    ImVec2 WindowPos = ImGui::GetCursorScreenPos();
    ImVec2 WindowSize = ImGui::GetContentRegionAvail();
    // if (ImPlot::BeginPlot("OctreeView")){

    // ImPlot::GetPlotDrawList()->AddRectFilled ImPlot::EndPlot();};
    /*
    for (int i= 0; i <= octree.GetDepth();i++)
    {
        uint32 Depth = i;
        auto Quads = QuadsToDraw[i];
        for (const auto &quad : Quads)
        {
            ImVec2 Min = quad.Min, Max = quad.Max;
            Min *= Size;
            Max *= Size;
            Min += WindowPos + ImVec2(ViewPos.x, ViewPos.y);
            Max += WindowPos + ImVec2(ViewPos.x, ViewPos.y);
            if (quad.Leaf || Filled)
            {
                ImGui::GetWindowDrawList()->AddRectFilled(Min, Max, ImGui::ColorConvertFloat4ToU32(quad.Color),
                                                          Rounding);
            }
            else
            {
                ImGui::GetWindowDrawList()->AddRect(Min, Max, ImGui::ColorConvertFloat4ToU32(quad.Color),
    Rounding,0,quad.width);
            }
        }
    }*/
    ImGui::SliderFloat("Wdith", &WidthModifier, 0.0f, 10.0f);

    if (ImGui::SliderInt2("Range", &RangeView.x, 0, octree.GetDepth()))
    {
        RangeView.x = glm::min(RangeView.x, RangeView.y);
        RangeView.y = glm::max(RangeView.x, RangeView.y);
    }
    ImGui::Text("Total Node Count: %d", NodeCount);
    const uint64 Res = octree.OctreeResolution();
    ImGui::Text("Resolution %ldx%ldx%ld", Res, Res, Res);
    const size_t sparseMemoryUse = octree.CurrentMemoryUsage();
    ImGui::Text("Memory Use Octree: %s", string_formatBytes(sparseMemoryUse).c_str());

    const size_t DenseMemoryUse = Res * Res * Res * sizeof(decltype(octree)::LeafDataType);
    ImGui::Text("Memory Use Dense Potential: %s", string_formatBytes(DenseMemoryUse).c_str());
    ImGui::Text("Memory Use Ratio: %f%%", (float(sparseMemoryUse) / DenseMemoryUse) * 100);
    ImGui::Checkbox("Filled", &Filled);
    if (ImGui::RadioButton("XY", AxisView == 0))
        AxisView = 0;
    ImGui::SameLine();
    if (ImGui::RadioButton("XZ", AxisView == 1))
        AxisView = 1;
    ImGui::SameLine();
    if (ImGui::RadioButton("YZ", AxisView == 2))
        AxisView = 2;

    if (ImPlot::BeginPlot("OctreeView", ImVec2(-1, -1), ImPlotFlags_NoLegend | ImPlotFlags_Equal))
    {
        const int32 Res = octree.OctreeResolution();
        ImPlot::SetupAxisLimits(ImAxis_X1, -Res / 2, Res + Res / 2);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, -Res / 2, Res + Res / 2);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -Res / 2, Res + Res / 2);
        ImPlot::PushPlotClipRect();
        for (int i = 0; i <= octree.GetDepth(); i++)
        {
            uint32 Depth = i;
            auto Quads = QuadsToDraw[i];
            for (const auto &quad : Quads)
            {
                ImVec2 Min = quad.Min, Max = quad.Max;

                // Min *= Size;
                // Max *= Size;
                // Min += WindowPos + ImVec2(ViewPos.x, ViewPos.y);
                // Max += WindowPos + ImVec2(ViewPos.x, ViewPos.y);
                if (quad.Leaf || Filled)
                {
                    
                    ImPlot::GetPlotDrawList()->AddRectFilled(ImPlot::PlotToPixels(Min), ImPlot::PlotToPixels(Max),
                                                             ImGui::ColorConvertFloat4ToU32(quad.Color), 0.0f);
                }
                else
                {

                    ImPlot::GetPlotDrawList()->AddRect(ImPlot::PlotToPixels(Min), ImPlot::PlotToPixels(Max),
                                                       ImGui::ColorConvertFloat4ToU32(quad.Color), 0.0f, 0, quad.width);
                }
            }
        }
        ImPlot::PopPlotClipRect();
        // ImPlot::PlotLine()
        ImPlot::EndPlot();
    }

    /*
     octree.ForEach([&](const decltype(octree)::NodeData &data) {
        const auto nodeSize = octree.SizeOfNodeAtDepth(data.Depth);
        vec3 Min = {data.NodePos}, Max = {Min + vec3(nodeSize)};
        Max = Max / (Range * 2);
        Min -= 1.0f;
        Max -= 1.0f;
        vec2 v000 = ProjMat * ViewMat * vec4{Min.x, Min.y, Min.z, 1.0f}; // (x=Min, y=Min, z=Min)
        vec2 v100 = ProjMat * ViewMat * vec4{Max.x, Min.y, Min.z, 1.0f};
        vec2 v110 = ProjMat * ViewMat * vec4{Max.x, Max.y, Min.z, 1.0f};
        vec2 v010 = ProjMat * ViewMat * vec4{Min.x, Max.y, Min.z, 1.0f};
        vec2 v001 = ProjMat * ViewMat * vec4{Min.x, Min.y, Max.z, 1.0f};
        vec2 v101 = ProjMat * ViewMat * vec4{Max.x, Min.y, Max.z, 1.0f};
        vec2 v111 = ProjMat * ViewMat * vec4{Max.x, Max.y, Max.z, 1.0f};
        vec2 v011 = ProjMat * ViewMat * vec4{Min.x, Max.y, Max.z, 1.0f};
        ImVec2 windowPos = ImGui::GetCursorScreenPos();
        auto DrawRect = [&](vec2 p0, vec2 p1, vec2 p2, vec2 p3) {
            auto GlmToImGui = [&](vec2 p) {
                p *= vec2(WindowSize.x, WindowSize.y);
                p += vec2(WindowPos.x, WindowPos.y);
                ImVec2 ip = {p.x, p.y};
                return ip;
            };

            draw_list->AddQuadFilled(GlmToImGui(p0), GlmToImGui(p1), GlmToImGui(p2), GlmToImGui(p3),
                                     ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.025f)));
        };
        DrawRect(v001, v101, v111, v011);
        DrawRect(v100, v000, v010, v110);
        DrawRect(v101, v100, v110, v111);
        DrawRect(v000, v001, v011, v010);
        DrawRect(v011, v111, v110, v010);
        DrawRect(v000, v100, v101, v001);
        // ImGui::GetWindowDrawList()->AddRectFilled(min + windowPos, max + windowPos,
        //                                           ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.05f)), 1.0f);
    });*/
}

AtlasDebugViewport::AtlasDebugViewport() : Editor::WindowToolGeneric("Atlas Debug Viewport")
{
    // octree.InsertLeaf({0,0,0});
    // octree.InsertLeaf({1,1,0});
    // octree.InsertLeaf({2,2,0});
    // return;
    const auto Res = octree.OctreeResolution();
    for (int i = 0; i < Res; i++)
    {
        octree.InsertLeaf({i, i, 0});
        octree.InsertLeaf({Res - i - 1, i, 0});
        for (int j = 0; j < Res; j++)
        {
        }
    }
    octree.Clear();
    std::vector<uvec3> Positions;
    const uint32 Resolution = octree.OctreeResolution();
    int center = Resolution / 2;
    float radius = (Resolution / 2) - 2.0f;
    float radiusSquared = radius * radius;
    for (int x = 0; x < Resolution; ++x)
    {
        for (int y = 0; y < Resolution; ++y)
        {
            if (false)
            {
                for (int z = 0; z < Resolution; ++z)
                {
                    int dx = x - center;
                    int dy = y - center;
                    int dz = z - center;
                    float distSquared = dx * dx + dy * dy + dz * dz;

                    // Surface check: distance within [r-0.5, r+0.5]
                    if (distSquared >= (radius - 0.5f) * (radius - 0.5f) &&
                        distSquared <= (radius + 0.5f) * (radius + 0.5f))
                    {
                        Positions.push_back(uvec3(x, y, z));
                    }
                }
            }
            else
            {
                int dx = x - center;
                int dy = y - center;

                float distSquared = dx * dx + dy * dy;

                // Surface check: distance within [r-0.5, r+0.5]
                if (distSquared >= (radius - 0.5f) * (radius - 0.5f) &&
                    distSquared <= (radius + 0.5f) * (radius + 0.5f))
                {
                    Positions.push_back(uvec3(x, y, x));
                }
            }

        }
    }
    octree.InsertLeafs(Positions);
    std::cerr << "Inserted " << Positions.size() << std::endl;
    /*
    MortonCodesTest();
    std::cerr << "Max MemoryUsage:" << string_formatBytes(octree.AbsMaxMemoryUse()) << std::endl;
    std::cerr << octree.OctreeResolution() << std::endl;
    octree.InsertLeaf({127, 25, 512});
    octree.InsertLeaf({2, 247, 85});
    auto start = std::chrono::high_resolution_clock::now();


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cerr << "total time: " << duration << std::endl;

    std::vector<uvec3> VoxelsToAdd;
    for (int i = 0; i < octree.OctreeResolution(); i++)
    {
        for (int j = 0; j < octree.OctreeResolution(); j++)
        {
            VoxelsToAdd.push_back({i, i, j});
        }
    }
    octree.Clear();
    start = std::chrono::high_resolution_clock::now();
    octree.InsertLeafs(VoxelsToAdd);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cerr << "total time: " << duration << std::endl;
    std::cerr << "Current MemoryUsage:" << string_formatBytes(octree.CurrentMemoryUsage()) << std::endl;
    boost::container::flat_map<int, int> a;
*/
    /*
        octree.Clear();
        std::vector<uvec3> Positions;
        int center = octree.OctreeResolution() / 2;
        float radius = (octree.OctreeResolution() / 2) - 2.0f;
        float radiusSquared = radius * radius;
        for (int x = 0; x < octree.OctreeResolution(); ++x)
        {
            for (int y = 0; y < octree.OctreeResolution(); ++y)
            {
                for (int z = 0; z < octree.OctreeResolution(); ++z)
                {
                    int dx = x - center;
                    int dy = y - center;
                    int dz = z - center;
                    float distSquared = dx * dx + dy * dy + dz * dz;

                    // Surface check: distance within [r-0.5, r+0.5]
                    if (distSquared >= (radius - 0.5f) * (radius - 0.5f) &&
                        distSquared <= (radius + 0.5f) * (radius + 0.5f))
                    {
                        Positions.push_back(uvec3(x, y, z));
                    }
                }
            }
        }*/
    // start = std::chrono::high_resolution_clock::now();
    // octree.InsertLeafs(Positions);
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cerr << "total time: " << duration << std::endl;
    std::cerr << "Current MemoryUsage:" << string_formatBytes(octree.CurrentMemoryUsage()) << std::endl;
}