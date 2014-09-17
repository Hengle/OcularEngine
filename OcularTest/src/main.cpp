/**
 * Copyright 2014 Steven T Sell (ssell@ocularinteractive.com)
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

#include <iostream>
#include "Renderer\Window\Window.hpp"

//------------------------------------------------------------------------------------------

void testOut()
{
    std::cout << "... Done!" << std::endl;
}

template<typename T, typename... Args>
void testOut(T first, Args... args)
{
    std::cout << first << std::endl;
    testOut(args...);
}

int main(int argc, char** argv)
{
    testOut("message");
    /*Ocular::WindowManager manager;
    
    if(manager.createWindow("Test", 800, 600, 8, 8, 8, Ocular::WINDOW_DISPLAY_MODE::WINDOWED_BORDERED))
    {
        manager.destroyWindow("Test");
    }
    else 
    {
        std::cout << "Failed to create window" << std::endl;
    }*/

    return 0;
}