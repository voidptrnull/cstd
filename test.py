import os
import subprocess

GREEN = '\033[92m'
RED = '\033[91m'
CYAN = '\033[96m'
YELLOW = '\033[93m'
RESET = '\033[0m'

def run_tests(test_dir):
    executables = [f for f in os.listdir(test_dir) if os.access(os.path.join(test_dir, f), os.X_OK) and os.path.isfile(os.path.join(test_dir, f))]
    
    total_tests = len(executables)
    passed = 0
    failed = 0

    print(f"{CYAN}Running tests in directory: {YELLOW}{test_dir}{RESET}")
    
    for exe in executables:
        exe_path = os.path.join(test_dir, exe)
        print(f"{GREEN}Running: {exe}{RESET}")
        
        result = subprocess.run([exe_path], capture_output=True, text=True)
        if result.returncode == 0:
            print(f"{GREEN}Test Passed: {exe}{RESET}")
            passed += 1
        else:
            print(f"{RED}Test Failed: {exe}{RESET}")
            print(f"{RED}Return Code: {result.returncode}{RESET}")
            print(f"{RED}Output: {result.stdout}{RESET}")
            print(f"{RED}Error: {result.stderr}{RESET}")
            failed += 1

    print(f"{CYAN}\nTest Summary:{RESET}")
    print(f"Total Tests: {total_tests}")
    print(f"{GREEN}Passed: {passed}{RESET}")
    print(f"{RED}Failed: {failed}{RESET}")

if __name__ == "__main__":
    current_dir = os.path.dirname(os.path.abspath(__file__))
    run_tests(os.path.join(current_dir, "build"))
