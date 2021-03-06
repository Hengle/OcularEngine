/**
* Copyright 2014-2015 Steven T Sell (ssell@vertexfragment.com)
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

#include "Common.hpp"
#include "FileIO/File.hpp"
#include "FileIO/Directory.hpp"
#include "Exceptions/FileReadWriteException.hpp"
#include "Utilities/StringRegistrar.hpp"

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <algorithm>

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Core
    {
        OCULAR_REGISTER_TO_STRING(File, OCULAR_TO_STRING_LAMBDA
        {
            std::string result;

            if(raw)
            {
                if(!isPointer)
                {
                    File file = void_cast<File>(raw);
                    result = file.getFullPath();
                }
                else
                {
                    File* file = void_cast<File*>(raw);

                    if(file)
                    {
                        result = file->getFullPath();
                    }
                }
            }

            return result;
        });

        OCULAR_REGISTER_FROM_STRING(File, OCULAR_FROM_STRING_LAMBDA
        {
            if(out)
            {
                File* result = void_cast<File*>(out); 

                if(result)
                {
                    result->setPath(str);
                }
            }
        });


        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        File::File(std::string const path)
        {
            if(path.empty())
            {
                m_FullPath = boost::filesystem::current_path().generic_string();
            }
            else 
            {
                m_FullPath = boost::filesystem::system_complete(boost::filesystem::path(path)).generic_string();
            }
            
            m_IsReal = false;
            m_IsReadable = false;
            m_IsWritable = false;
            m_IsFile = false;
            m_IsDirectory = false;
            m_IsSymLink = false;
            m_IsAbsolute = false;
            m_FileSize = 0;

            refresh();
        }

        File::~File()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        void File::setPath(std::string const path)
        {
            m_FullPath = path;
            refresh();
        }

        void File::refresh()
        {
            formatForSystem();

            boost::filesystem::path file(m_FullPath.c_str());

            m_IsReal = boost::filesystem::exists(file);

            if(m_IsReal)
            {
                m_IsFile = boost::filesystem::is_regular_file(file);
                m_IsDirectory = boost::filesystem::is_directory(file);
                m_IsSymLink = boost::filesystem::is_symlink(file);
                m_IsAbsolute = file.is_absolute();

                if(m_IsFile)
                {
                    m_FileSize = static_cast<unsigned>(boost::filesystem::file_size(file));

                    // Readable?
                    std::ifstream inStream(m_FullPath);
                    m_IsReadable = inStream.good();
                    inStream.close();

                    // Writable?
                    std::ofstream outStream(m_FullPath, std::ios_base::app);
                    m_IsWritable = outStream.good();
                    outStream.close();
                }
            }

            // Break up path
            m_Extension = file.extension().string();
            m_Name = file.filename().string();

            if(m_IsFile)
            {
                m_Name = m_Name.substr(0, m_Name.find(m_Extension));  // Remove the extension from the name
                m_Directory = file.remove_filename().string();
            }
            else 
            {
                m_Directory = m_FullPath;
                std::size_t sepPos = m_Directory.find_last_of(OCULAR_PATH_SEPARATOR);

                if(sepPos != std::string::npos)
                {
                    m_Directory = m_Directory.substr(0, sepPos);
                }
            }
        }

        bool File::exists() const
        {
            return m_IsReal;
        }

        bool File::canRead() const
        {
            return m_IsReadable;
        }

        bool File::canWrite() const
        {
            return m_IsWritable;
        }

        bool File::isFile() const
        {
            return m_IsFile;
        }

        bool File::isDirectory() const
        {
            return m_IsDirectory;
        }

        bool File::isSymLink() const
        {
            return m_IsSymLink;
        }

        bool File::isAbsolute() const
        {
            return m_IsAbsolute;
        }

        unsigned File::getSize() const
        {
            return m_FileSize;
        }

        std::string File::getFullPath() const
        {
            return m_FullPath;
        }

        std::string File::getName() const
        {
            return m_Name;
        }

        std::string File::getExtension() const
        {
            return m_Extension;
        }

        std::string File::getDirectory() const
        {
            return m_Directory;
        }

        time_t File::getLastModifiedTime() const
        {
            time_t result = 0;

            if(m_IsFile)
            {
                boost::filesystem::path file(m_FullPath);
                result = boost::filesystem::last_write_time(file);
            }

            return result;
        }

        void File::formatForSystem()
        {
#ifdef OCULAR_WINDOWS
            std::replace(m_FullPath.begin(), m_FullPath.end(), '/', '\\');
            std::replace(m_Directory.begin(), m_Directory.end(), '/', '\\');
#else
            m_FullPath.replace(m_FullPath.begin(), m_FullPath.end(), '\\', '//');
            m_Directory.replace(m_Directory.begin(), m_Directory.end(), '\\', '//');
#endif
        }

        bool File::create(bool createDirectories)
        {
            bool result = false;

            if(!m_IsReal)
            {
                if(createDirectories)
                {
                    Directory directory(m_Directory);
                    directory.create(true);
                }

                std::ofstream stream(m_FullPath, std::ios::out);
                
                if(stream.is_open())
                {
                    stream << "";

                    stream.close();
                    refresh();

                    result = true;
                }
            }

            return result;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}