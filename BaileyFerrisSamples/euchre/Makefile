CXX = g++
CXXFLAGS = -Wall -Werror -pedantic

all: release

debug: CXXFLAGS += -g
debug: test
release: CXXFLAGS += -O1
release: test

test: Card_tests Pack_tests Player_tests Card_public_test Pack_public_test Player_public_test euchre
	./Card_tests
	./Pack_tests
	./Player_tests
	./Card_public_test
	./Pack_public_test
	./Player_public_test

	./euchre pack.in noshuffle 1 Alice Simple Bob Simple Cathy Simple Drew Simple > euchre_test00.out
	diff -q euchre_test00.out euchre_test00.out.correct
	./euchre pack.in shuffle 10 Alice Simple Bob Simple Cathy Simple Drew Simple > euchre_test01.out
	diff -q euchre_test01.out euchre_test01.out.correct
	./euchre pack.in noshuffle 1 Alice Human Bob Human Cathy Human Drew Human < euchre_test50.in > euchre_test50.out
	diff -q euchre_test50.out euchre_test50.out.correct


Card_public_test: Card.cpp Card_public_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Card_tests: Card.cpp Card_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@	

Pack_public_test: Card.cpp Pack.cpp Pack_public_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Pack_tests: Card.cpp Pack.cpp Pack_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@	

Player_public_test: Card.cpp Player.cpp Player_public_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

Player_tests: Card.cpp Player.cpp Player_tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@	

euchre: Card.cpp Pack.cpp Player.cpp euchre.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@


.PHONY: clean
clean:
	rm -vf *.out euchre Card_public_test Pack_public_test Player_public_test \
			Card_tests Pack_tests Player_tests
