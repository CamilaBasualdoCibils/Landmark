#pragma once
#include <pch.h>


namespace Render
{
    /**
     * @brief This defines the context of a Rendering operation. This is the final output. 
     * 
     */
    class CompositePipeline
    {
        std::vector<std::shared_ptr<CompositeLayer>> Layers;
        public:
        //virtual void Compose(CompositePayload& p) =0;
        template <typename T>
        std::shared_ptr<T> Push()
        {
            std::shared_ptr<T> newObject = std::make_shared<T>();
            Layers.push_back(newObject);
            return newObject;
        }

        /**
         * @brief A collection of layers that happen in sequence
         * 
         * @tparam Payload 
         */

    };
} // namespace Render
