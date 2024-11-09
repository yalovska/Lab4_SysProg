#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

void createAutomatonFile(const string &filename, const vector<string> &content) {
    ofstream file(filename);
    for (const string &line : content) {
        file << line << endl;
    }
    file.close();
}

bool runTest(const string &filename, int k, const string &expectedOutput) {
    string command = "./main " + filename + " " + to_string(k) + " > output.txt";
    system(command.c_str());

    ifstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Не вдалося відкрити файл output.txt" << endl;
        return false;
    }

    string line, actualOutput;
    while (getline(outputFile, line)) {
        actualOutput += line + "\n";
    }
    outputFile.close();

    return actualOutput == expectedOutput;
}

int main() {
    vector<string> tests = {
            "Тест 1: Приймає всі слова довжини 2",
            "Тест 2: Не приймає всі слова довжини 3",
            "Тест 3: Приймає порожнє слово"
    };

    vector<vector<string>> testContents = {
            {"2", "a b", "2", "0 1", "0", "1", "0 a 1", "0 b 1", "1 a 1", "1 b 1"},
            {"2", "a b", "3", "0 1 2", "0", "2", "0 a 1", "1 b 2"},
            {"1", "a", "1", "0", "0", "0"}
    };

    vector<int> testK = {2, 3, 0};
    vector<string> expectedOutputs = {
            "Автомат сприймає всі слова довжини 2\n",
            "Автомат не сприймає всі слова довжини 3\n",
            "Автомат сприймає всі слова довжини 0 (порожнє слово).\n"
    };

    bool allTestsPassed = true;

    for (int i = 0; i < tests.size(); i++) {
        string filename = "tests/test_automaton_" + to_string(i) + ".txt";
        createAutomatonFile(filename, testContents[i]);

        cout << tests[i] << ": ";
        if (runTest(filename, testK[i], expectedOutputs[i])) {
            cout << "✅" << endl;
        } else {
            cout << "❌" << endl;
            allTestsPassed = false;
        }
    }

    if (allTestsPassed) {
        cout << "Усі тести пройдено успішно!" << endl;
    } else {
        cout << "Деякі тести не пройдено. Перевірте помилки." << endl;
    }

    return 0;
}
