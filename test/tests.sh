# RUN DIFFERENT TESTS

# FUNCTION TO RUN TESTS
function testFile {
    if [ "`bin/kintex $1 2>&1`" = "$2" ]; then
        echo "->succesful!"
    else
        echo "->FAILED!"
    fi
}
function testCommand {
    if [ "`bin/kintex -c $1 2>&1`" = "$2" ]; then
        echo "->succesful!"
    else
        echo "->FAILED!"
    fi
}

echo "-->Running (mathematical) tests..."
echo "Testing addition... ";
testCommand "2+3+4+5" "14"
echo "Testing subtraction (and negative numbers)... ";
testCommand "2-3-4--5" "0"
echo "Testing multiplication... ";
testCommand "2*3*4*5" "120"
echo "Testing divide (and floating-point)... ";
testCommand "5/4/2" "0.625"
echo "Testing exponent ... ";
testCommand "5^2^3" "390625"
echo "Testing void... ";
testCommand "(void);2+2;" "4"
echo "Testing variables (and set)... ";
testFile test/variables.kin "6"
echo "Testing functions... ";
testFile test/functions.kin "194.4"
echo "Testing parentheses";
testCommand "(4+2)/3" "2"
echo "Testing brackets";
testFile test/brackets.kin "7"
echo "Testing core library";
testFile test/library.kin "4037913"
echo "Testing builtin functions";
testCommand "sqrt(9)" "3"
echo "Testing scopes";
testFile test/scope.kin "-197"
echo "Running total test (combination of features)... ";
testFile test/total.kin "720.938"
echo "-->Tests finished"
