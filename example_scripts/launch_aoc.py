import os
import time
import psutil
import subprocess
from ctypes import windll

# this script launches an aoc instance and loads the aoc-auto-game dll
# note that this works on 32-bit python only
dll_path = b'../Release/aoc-auto-game.dll'
aoc_name = "age2_x1.exe"
aoc_path = os.getenv('APPDATA') + "/Microsoft Games/Age of Empires ii/Age2_x1/" + aoc_name

# kill any previous aoc processes
aoc_procs = [proc for proc in psutil.process_iter() if proc.name() == aoc_name]
for aoc_proc in aoc_procs: aoc_proc.kill()

# launch aoc and wait for it to init
aoc_proc = subprocess.Popen(aoc_path)
time.sleep(4)

# write dll path into aoc memory
aoc_handle = windll.kernel32.OpenProcess(0x1FFFFF, False, aoc_proc.pid) # PROCESS_ALL_ACCESS
remote_memory = windll.kernel32.VirtualAllocEx(aoc_handle, 0, 260, 0x3000, 0x40)
windll.kernel32.WriteProcessMemory(aoc_handle, remote_memory, dll_path, len(dll_path), 0)

# load the dll from the remote process
load_library = windll.kernel32.GetProcAddress(windll.kernel32._handle, b'LoadLibraryA')
remote_thread = windll.kernel32.CreateRemoteThread(aoc_handle, 0, 0, load_library, remote_memory, 0, 0)
windll.kernel32.WaitForSingleObject(remote_thread, 0xFFFFFFFF)
windll.kernel32.CloseHandle(remote_thread)

# clean up
windll.kernel32.VirtualFreeEx(aoc_handle, remote_memory, 0, 0x00008000)
windll.kernel32.CloseHandle(aoc_handle)
