dash : Main.c RaiseError/RaiseError.c Utils/ArrayUtils.c Utils/StringUtils.c Execution/Execution.c Prompt/Prompt.c
	gcc -o dash Main.c RaiseError/RaiseError.c Utils/ArrayUtils.c Utils/StringUtils.c Execution/Execution.c Prompt/Prompt.c -Wall -Wextra -Werror -std=c17

dashTests : Test/UnitTests.c Utils/ArrayUtils.c Utils/StringUtils.c Execution/Execution.c RaiseError/RaiseError.c
	gcc -o dashTests Test/UnitTests.c Utils/ArrayUtils.c Utils/StringUtils.c Execution/Execution.c RaiseError/RaiseError.c -Wall -Wextra -Werror -std=c17