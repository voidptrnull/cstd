import os

def run():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(current_dir)
    os.system("doxygen Doxyfile")
    
if __name__ == '__main__':
    run()