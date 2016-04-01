/**
 * Copyright 2014-2016 Steven T Sell (ssell@ocularinteractive.com)
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

#include "Resources/Resource.hpp"
#include "Utilities/StringRegistrar.hpp"
#include "Utilities/VoidCast.hpp"

#include "OcularEngine.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Core
    {
        OCULAR_REGISTER_TO_STRING(Resource, OCULAR_TO_STRING_LAMBDA
        {
            std::string result;

            if(raw)
            {
                Resource* resource = void_cast<Resource*>(raw);

                if(resource)
                {
                    result = resource->getMappingName();
                }
            }

            return result;
        });

        OCULAR_REGISTER_FROM_STRING(Resource, OCULAR_FROM_STRING_LAMBDA
        {
            if(out)
            {
                Resource** resource = (Resource**)(out);

                if(resource)
                {
                    *resource = OcularResources->getResource(str);
                }
            }
        });

        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        Resource::Resource(File const& source)
            : m_SourceFile(source),
              m_Name(source.getName()),
              m_IsInMemory(false),
              m_SizeInMemory(0),
              m_Type(ResourceType::Undefined)
        {

        }

        Resource::Resource()
            : m_SourceFile(File()),
              m_Name(""),
              m_IsInMemory(false),
              m_SizeInMemory(0),
              m_Type(ResourceType::Undefined)
        {

        }

        Resource::~Resource()
        {
        
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        void Resource::unload()
        {

        }

        void Resource::forceLoad()
        {
            /// \todo Resources forceLoad
        }

        File Resource::getSourceFile() const 
        {
            return m_SourceFile;
        }

        void Resource::setSourceFile(File const& file)
        {
            m_SourceFile = file;
        }

        void Resource::setIsInMemory(bool inMemory) 
        {
            m_IsInMemory = inMemory;
        }

        bool Resource::isInMemory() const
        {
            return m_IsInMemory;
        }

        uint64_t Resource::getSize() const
        {
            return m_SizeInMemory;
        }

        ResourceType Resource::getResourceType() const
        {
            return m_Type;
        }

        void Resource::setName(std::string const& name)
        {
            m_Name = name;
        }

        std::string const& Resource::getName() const
        {
            return m_Name;
        }

        std::string const& Resource::getMappingName() const
        {
            return m_MappingName;
        }

        void Resource::setMappingName(std::string const& name)
        {
            m_MappingName = name;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------

    }
}