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

#include "stdafx.h"
#include "Routines/EditorCameraController.hpp"

#include "Scene/RoutineRegistrar.hpp"
#include "Scene/SceneObject.hpp"
#include "Events/Events/MouseScrollInputEvent.hpp"
#include "Math/Bounds/Ray.hpp"

#include "OcularEngine.hpp"

OCULAR_REGISTER_ROUTINE(Ocular::Editor::EditorCameraController, "EditorCameraController")

namespace
{
    enum CameraMode
    {
        Default = 0,
        Drag,
        Look,
        Pan
    };

    const float FocusProcessTime = 0.25f;       // Amount of time, in seconds, to process a camera focus move event
}

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        EditorCameraController::EditorCameraController()
            : ARoutine("EditorCameraController", "EditorCameraController"),
              m_Mode(CameraMode::Default),
              m_LookSensitivity(0.001f),
              m_PanSensitivity(0.001f),
              m_ZoomSensitivity(0.001f),          // Scroll events typically generate deltas of 120, so default to treat as 0.12
              m_IsMovingFocus(false),
              m_FocusElapsed(0.0f)
        {
            OcularEvents->registerListener(this, Core::Priority::Medium);
        }

        EditorCameraController::~EditorCameraController()
        {
            OcularEvents->unregisterListener(this);
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // Inherited Methods
        //----------------------------------------------------------------------------------

        void EditorCameraController::onUpdate(float const delta)
        {
            ARoutine::onUpdate(delta);

            if(m_Parent)
            {
                updateFocusMove(delta);
                updateCameraMode();
                handleMouseMovement();
            }
        }

        bool EditorCameraController::onEvent(std::shared_ptr<Core::AEvent> event)
        {
            if(event->isType<Core::MouseScrollInputEvent>())
            {
                Core::MouseScrollInputEvent* scrollEvent = dynamic_cast<Core::MouseScrollInputEvent*>(event.get());

                if(m_Parent)
                {
                    // Use -delta so that scrolling 'forward' moves the camera forward
                    Math::Vector3f movement = Math::Vector3f(0.0f, 0.0f, (static_cast<float>(-scrollEvent->delta) * m_ZoomSensitivity));
                    m_Parent->translate(movement);
                }
            }

            return true;
        }

        //----------------------------------------------------------------------------------
        // Controller Specific Methods
        //----------------------------------------------------------------------------------

        void EditorCameraController::focus(Core::SceneObject const* object)
        {
            if(object)
            {
                if(m_Parent)
                {
                    const Math::Matrix4x4 objMatrix = object->getModelMatrix(false);

                    const Math::Vector4f objCenter = objMatrix * Math::Vector4f(object->boundsAABB.getCenter());
                    const Math::Vector3f forward = m_Parent->getTransform().getForwards().getNormalized();

                    const float offset = std::max(1.0f, object->boundsSphere.getRadius() * 5.0f);
                    const Math::Ray ray = Math::Ray(Math::Vector3f(objCenter.x, objCenter.y, objCenter.z), forward);

                    m_FocusStartPos = m_Parent->getTransform().getPosition();
                    m_FocusEndPos   = ray.getPointAlong(offset);
                    m_IsMovingFocus = true;
                    m_FocusElapsed  = FocusProcessTime;
                }
            }
        }

        void EditorCameraController::setLookSensitivity(float sensitivity)
        {
            m_LookSensitivity = sensitivity;
        }

        float EditorCameraController::getLookSensitivity() const
        {
            return m_LookSensitivity;
        }

        void EditorCameraController::setPanSensitivity(float sensitivity)
        {
            m_PanSensitivity = sensitivity;
        }

        float EditorCameraController::getPanSensitivity() const
        {
            return m_PanSensitivity;
        }

        void EditorCameraController::setZoomSensitivity(float speed)
        {
            m_ZoomSensitivity = speed;
        }

        float EditorCameraController::getZoomSensitivity() const
        {
            return m_ZoomSensitivity;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        void EditorCameraController::updateFocusMove(float const delta)
        {
            if(m_IsMovingFocus)
            {
                if(m_FocusElapsed > 0.0f)
                {
                    m_FocusElapsed -= delta;

                    if(m_Parent)
                    {
                        const float fractional = (FocusProcessTime - m_FocusElapsed) / FocusProcessTime;
                        const Math::Vector3f currPos = Math::Vector3f::Lerp(m_FocusStartPos, m_FocusEndPos, fractional);

                        m_Parent->getTransform().setPosition(currPos);
                    }
                }
                else
                {
                    m_FocusElapsed = 0.0f;
                    m_IsMovingFocus = false;
                }
            }
        }

        void EditorCameraController::updateCameraMode()
        {
            switch(m_Mode)
            {
            case CameraMode::Default:
            {
                if(OcularInput->isMouseButtonDown(Core::MouseButtons::Left))
                {
                    m_Mode = CameraMode::Drag;
                    m_LastMousePos = OcularInput->getMousePosition();
                }
                else if(OcularInput->isMouseButtonDown(Core::MouseButtons::Right))
                {
                    m_Mode = CameraMode::Look;
                    m_LastMousePos = OcularInput->getMousePosition();
                }
                else if(OcularInput->isMouseButtonDown(Core::MouseButtons::Middle))
                {
                    m_Mode = CameraMode::Pan;
                    m_LastMousePos = OcularInput->getMousePosition();
                }

                break;
            }

            case CameraMode::Drag:
            {
                if(!OcularInput->isMouseButtonDown(Core::MouseButtons::Left))
                {
                    m_Mode = CameraMode::Default;
                }

                break;
            }

            case CameraMode::Look:
            {
                if(!OcularInput->isMouseButtonDown(Core::MouseButtons::Right))
                {
                    m_Mode = CameraMode::Default;
                }

                break;
            }

            case CameraMode::Pan:
            {
                if(!OcularInput->isMouseButtonDown(Core::MouseButtons::Middle))
                {
                    m_Mode = CameraMode::Default;
                }

                break;
            }

            default:
                break;
            }
        }

        void EditorCameraController::handleMouseMovement()
        {
            static const float DeltaMax = 200.0f;

            if(m_Mode != CameraMode::Default)
            {
                const Math::Vector2i currentPos = OcularInput->getMousePosition();

                m_DeltaVector.x = (static_cast<float>(currentPos.x) - static_cast<float>(m_LastMousePos.x));
                m_DeltaVector.y = (static_cast<float>(currentPos.y) - static_cast<float>(m_LastMousePos.y));

                if((m_DeltaVector.x > -DeltaMax) && (m_DeltaVector.x < DeltaMax) &&
                   (m_DeltaVector.y > -DeltaMax) && (m_DeltaVector.y < DeltaMax))
                {
                    m_IsMovingFocus = false;

                    switch(m_Mode)
                    {
                    case CameraMode::Drag:
                        handleMouseDrag();
                        break;

                    case CameraMode::Look:
                        handleMouseLook();
                        break;

                    case CameraMode::Pan:
                        handleMousePan();
                        break;

                    default:
                        break;
                    }
                }

                m_LastMousePos = currentPos;
            }
        }

        void EditorCameraController::handleMouseDrag()
        {

        }

        void EditorCameraController::handleMouseLook()
        {
            m_LookEuler.x += -m_DeltaVector.x * m_LookSensitivity;
            m_LookEuler.y += -m_DeltaVector.y * m_LookSensitivity;

            m_Parent->setRotation(Math::Quaternion(m_LookEuler));
        }

        void EditorCameraController::handleMousePan()
        {
            m_Parent->translate(m_DeltaVector * m_PanSensitivity);
        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}