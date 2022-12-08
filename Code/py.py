import os
import subprocess

path = "../Tests/Advanced/inputs"
out_path = "../Tests/Advanced/outputs"
for testcase in os.listdir(path):
    test_path = os.path.join(path, testcase)
    subprocess.run(["./parser", test_path, out_path+"/"+testcase+".s"])