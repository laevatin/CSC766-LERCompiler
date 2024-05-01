import os

testCasePathBase = "testcases/"

casePathDict = {}

try:
	allfiles = os.listdir(testCasePathBase)

	for file in allfiles:
		if file.endswith(".txt"):
			casePath = testCasePathBase + file
			casePathDict[casePath] = file[:-4]

	os.chdir("LER/")
	os.system("javac Glory.java DirectiveListener.java")
	for testCase in casePathDict.keys():
		os.system("java Glory ../" + testCase + " > ../testResult/" + casePathDict.get(testCase)+".result.txt")


except OSError as msg:
	print("can not open testcases directory")