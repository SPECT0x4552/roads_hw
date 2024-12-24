#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No color

# Test files
FILE1="082.txt"
FILE2="081.txt"

# Ensure test files exist
if [[ ! -f "$FILE1" || ! -f "$FILE2" ]]; then
  echo -e "${RED}Error: Test files $FILE1 or $FILE2 are missing!${NC}"
  exit 1
fi

echo -e "${YELLOW}Starting ./trails automated tests...${NC}"

# Test 1: Valid input with FILE1
echo -e "${YELLOW}Running test 1: Valid input with $FILE1...${NC}"
./trails --input=$FILE1
sleep 2

# Test 2: Valid input with FILE2
echo -e "${YELLOW}Running test 2: Valid input with $FILE2...${NC}"
./trails --input=$FILE2
sleep 2

# Test 3: Prompt = interactive with FILE1
echo -e "${YELLOW}Running test 3: Interactive prompt with $FILE1...${NC}"
echo -e "2.5" | ./trails --input=$FILE1 --prompt=interactive
sleep 2

# Test 4: Prompt = interactive with FILE2
echo -e "${YELLOW}Running test 4: Interactive prompt with $FILE2...${NC}"
echo -e "1.5" | ./trails --input=$FILE2 --prompt=interactive
sleep 2

# Test 5: Prompt disabled with FILE1
echo -e "${YELLOW}Running test 5: Disabled prompt with $FILE1...${NC}"
./trails --input=$FILE1 --prompt=disabled
sleep 2

# Test 6: Prompt disabled with FILE2
echo -e "${YELLOW}Running test 6: Disabled prompt with $FILE2...${NC}"
./trails --input=$FILE2 --prompt=disabled
sleep 2

# Test 7: Mismatched count with FILE1
echo -e "${YELLOW}Running test 7: Mismatched count with $FILE1...${NC}"
cp $FILE1 temp_082.txt
sed -i '1s/.*/4/' temp_082.txt # Introduce mismatch in trail count
./trails --input=temp_082.txt
rm temp_082.txt
sleep 2

# Test 8: Mismatched count with FILE2
echo -e "${YELLOW}Running test 8: Mismatched count with $FILE2...${NC}"
cp $FILE2 temp_081.txt
sed -i '1s/.*/3/' temp_081.txt # Introduce mismatch in trail count
./trails --input=temp_081.txt
rm temp_081.txt
sleep 2

# Test 9: Invalid file structure (missing or invalid trail count)
echo -e "${YELLOW}Running test 9: Invalid file structure test...${NC}"
echo -e "invalid\nRada2 6.19\n" > invalid_test.txt
./trails --input=invalid_test.txt || echo -e "${RED}Program exited as expected for invalid file structure.${NC}"
rm invalid_test.txt
sleep 2

# Test 10: Invalid trail data (negative trail length)
echo -e "${YELLOW}Running test 10: Invalid trail data test...${NC}"
echo -e "2\nRada2 -6.19\nMets 4.92\n" > invalid_trail_data.txt
./trails --input=invalid_trail_data.txt || echo -e "${RED}Program exited as expected for invalid trail data.${NC}"
rm invalid_trail_data.txt
sleep 2

# Test 11: Empty file
echo -e "${YELLOW}Running test 11: Empty file test...${NC}"
touch empty_test.txt
./trails --input=empty_test.txt || echo -e "${RED}Program exited as expected for empty input file.${NC}"
rm empty_test.txt
sleep 2

# Test 12: Display help manual --help
echo -e "${YELLOW}Running test 9: Displaying help manual (--help)...${NC}"
./trails --help
sleep 2

# Test 13: Display help manual --help
echo -e "${YELLOW}Running test 10: Displaying help manual (-h)...${NC}"
./trails -h
sleep 2

echo -e "${GREEN}All tests completed.${NC}"

