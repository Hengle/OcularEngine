/**
 * Copyright 2014-2015 Steven T Sell (ssell@ocularinteractive.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#ifndef __H__OCULAR_GRAPHICS_MESH__H__
#define __H__OCULAR_GRAPHICS_MESH__H__

#include "Resources/Resource.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

//------------------------------------------------------------------------------------------

/**
 * \addtogroup Ocular
 * @{
 */
namespace Ocular
{
    /**
     * \addtogroup Graphics
     * @{
     */
    namespace Graphics
    {
        /**
         * \class Mesh 
         */
        class Mesh : public Core::Resource
        {
        public:

            Mesh();
            ~Mesh();

            virtual void bind();
            virtual void unbind();

            virtual void unload() override;

            void setIndexBuffer(IndexBuffer* buffer);
            IndexBuffer const* getIndexBuffer() const;

            void setVertexBuffer(VertexBuffer* buffer);
            VertexBuffer const* getVertexBuffer() const;

        protected:

            IndexBuffer* m_IndexBuffer;
            VertexBuffer* m_VertexBuffer;

        private:
        };
    }
    /**
     * @} End of Doxygen Groups
     */
}
/**
 * @} End of Doxygen Groups
 */

//------------------------------------------------------------------------------------------

#endif