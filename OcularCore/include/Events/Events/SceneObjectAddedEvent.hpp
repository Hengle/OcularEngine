/**
 * Copyright 2014-2017 Steven T Sell (ssell@vertexfragment.com)
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
#ifndef __H__OCULAR_EVENTS_SCENE_OBJECT_ADDED_EVENT__H__
#define __H__OCULAR_EVENTS_SCENE_OBJECT_ADDED_EVENT__H__

#include "Events/AEvent.hpp"
#include "Scene/SceneObject.hpp"

//------------------------------------------------------------------------------------------

/**
* \addtogroup Ocular
* @{
*/
namespace Ocular
{
    /**
     * \addtogroup Core
     * @{
     */
    namespace Core
    {
        /**
         * \class SceneObjectAddedEvent
         *
         * Event notifying that a SceneObject has been added to the active Scene.
         *
         * String Descriptor: "SceneObjectAddedEvent" <br/>
         * Event Priority: Medium 
         */
        class SceneObjectAddedEvent : public AEvent 
        {
        public:

            SceneObjectAddedEvent(SceneObject* object);
            SceneObjectAddedEvent();

            virtual ~SceneObjectAddedEvent();

            SceneObject* object;
            Core::UUID uuid;

        protected:

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