# MIT License
#
# Copyright (c) 2024 Subhadip Roy Chowdhury
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

"""
A custom, minimal testing script with real-time streaming output.
"""

import os
import subprocess

GREEN = '\033[92m'
RED = '\033[91m'
CYAN = '\033[96m'
YELLOW = '\033[93m'
RESET = '\033[0m'
BRIGHT_WHITE = '\033[37;1m'

def stream_output(process):
    """Streams output from a running process in real time."""
    while process.poll() is None:
        output = process.stdout.readline()
        if output:
            print(output.strip())
    remaining_output = process.stdout.read()
    if remaining_output:
        print(remaining_output.strip())

def run_tests(test_dir):
    """Run all test executables in the specified directory."""
    executables = [
        f for f in os.listdir(test_dir) 
        if os.access(os.path.join(test_dir, f), os.X_OK) 
        and os.path.isfile(os.path.join(test_dir, f)) 
        and f[:1].isnumeric()
    ]

    total_tests = len(executables)
    passed, failed = 0, 0

    print(f"{CYAN}Running tests in directory: {YELLOW}{test_dir}{RESET}")

    for exe in executables:
        exe_path = os.path.join(test_dir, exe)
        print(f"\n{CYAN}Executing: {YELLOW}{exe}{RESET}...")

        process = subprocess.Popen(
            [exe_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
        )
        stream_output(process)

        process.wait()

        if process.returncode == 0:
            print(f"{GREEN}✔ Test Passed: {exe}{RESET}")
            passed += 1
        else:
            print(f"{RED}✘ Test Failed: {exe}{RESET}")
            print(f"{RED}Return Code: {BRIGHT_WHITE}{process.returncode}{RESET}")

            stdout, stderr = process.communicate()
            if stdout:
                print(f"{BRIGHT_WHITE}--- Output ---{RESET}\n{stdout}")
            if stderr:
                print(f"{BRIGHT_WHITE}--- Error ---{RESET}\n{stderr}")

            failed += 1

    print(f"\n{CYAN}Test Summary:{RESET}")
    print(f"Total Tests: {BRIGHT_WHITE}{total_tests}{RESET}")
    print(f"{GREEN}Passed: {BRIGHT_WHITE}{passed}{RESET}")
    print(f"{RED}Failed: {BRIGHT_WHITE}{failed}{RESET}")
    if total_tests > 0:
        failure_percent = (failed / total_tests) * 100.0
        print(f"{CYAN}Failure %: {BRIGHT_WHITE}{failure_percent:.2f}%{RESET}")
    else:
        print(f"{YELLOW}No tests found!{RESET}")

if __name__ == "__main__":
    current_dir = os.path.dirname(os.path.abspath(__file__))
    test_dir = os.path.join(current_dir, "build")
    run_tests(test_dir)
