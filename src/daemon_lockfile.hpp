/*
	Daemon Lockfile
	Repo: https://github.com/SiddiqSoft/daemon_lockfile

	BSD 3-Clause License

	Copyright (c) 2021, Siddiq Software LLC
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived from
	this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef DAEMON_LOCKFILE_HPP
#define DAEMON_LOCKFILE_HPP

#include <string>
#include <fstream>
#include <format>
#include <iostream>
#include <filesystem>
#include <process.h>


namespace siddiqsoft
{
	/// @brief The DaemonLockfile class allows you to include the instance in your executeable
	/// and check for `isLocked` prior to continuing your boot process.
	class DaemonLockfile
	{
	public:
		int									  processId {};
		std::string							  moduleName {};
		bool								  isLocked {};

	protected:
		int					  owningProcessId {};
		std::string			  owningModuleName {};
		std::filesystem::path directoryPath {};
		std::filesystem::path lockFilePath {};

		DaemonLockfile()					 = delete;
		DaemonLockfile(DaemonLockfile&& src) = delete;
		DaemonLockfile& operator=(DaemonLockfile&& src) = delete;

	public:
		/// @brief Automatically acquires a lock file if possible
		/// @param src The name of the process (may include extention: `my_daemon.exe` or `my_daemon.a` or simply `my_daemon`)
		DaemonLockfile(const std::string& src)
			: moduleName(src)
			, processId(_getpid())
		{
			using namespace std;

			try
			{
				directoryPath = filesystem::canonical(filesystem::path {filesystem::current_path()});
				lockFilePath  = format("{}\\{}.lockfile", directoryPath.string(), moduleName);

				if (filesystem::exists(lockFilePath))
				{
					// File already exists.. check if we're owner
					try
					{
						if (std::ifstream ifs {lockFilePath}; !ifs.eof())
						{
							ifs >> owningProcessId;
							ifs >> owningModuleName;
							isLocked = (processId == owningProcessId);
							ifs.close();
						}
					}
					catch (const std::exception& e)
					{
						isLocked = false;
					}
				}
				else
				{
					// Not found. Create the lockfile.
					if (std::ofstream ofs(lockFilePath); ofs.is_open())
					{
						// Write metadata
						owningProcessId = processId;
						// Three lines: 1) owning processid 2) moduleName 3) timestamp of lock
						ofs << owningProcessId << std::endl
							<< moduleName << std::endl
							<< format("{:%FT%T}Z", std::chrono::system_clock::now()) << std::endl;
						ofs.close();
						// We're owners
						isLocked = true;
					}
				}
			}
			catch (...)
			{
			}
		}

		/// @brief Release the lock if possible
		~DaemonLockfile()
		{
			try
			{
				if (isLocked && (owningProcessId == processId))
				{
					// This lockfile is by the current process, release it
					std::filesystem::remove(lockFilePath);
				}
			}
			catch (const std::exception&)
			{
			}
		}
	};
} // namespace siddiqsoft

#endif
