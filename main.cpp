#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

struct FiniteAutomaton {
    set<char> alphabet;
    set<int> states;
    int start_state = 0;
    set<int> final_states;
    unordered_map<int, unordered_map<char, set<int>>> transitions;

    void addTransition(int from, char input, int to) {
        transitions[from][input].insert(to);
    }

    [[nodiscard]] bool accepts(const string &word, int index, int currentState) const {
        if (index == word.size()) {
            bool isFinal = final_states.find(currentState) != final_states.end();
            cout << "Результат для слова \"" << word << "\" у стані " << currentState << ": " << (isFinal ? "Приймається" : "Не приймається") << endl;
            return isFinal;
        }

        char symbol = word[index];
        if (transitions.find(currentState) == transitions.end() || transitions.at(currentState).find(symbol) == transitions.at(currentState).end()) {
            cout << "Немає переходу для символу '" << symbol << "' у стані " << currentState << endl;
            return false;
        }

        return std::any_of(
                transitions.at(currentState).at(symbol).begin(),
                transitions.at(currentState).at(symbol).end(),
                [&](int nextState) { return accepts(word, index + 1, nextState); }
        );
    }

    [[nodiscard]] bool acceptsEmptyWord() const {
        set<int> visited;
        bool result = canReachFinalState(start_state, visited);
        cout << "Приймає порожнє слово: " << (result ? "Так" : "Ні") << endl;
        return result;
    }

    bool canReachFinalState(int currentState, set<int>& visited) const {
        if (final_states.find(currentState) != final_states.end()) {
            return true;
        }
        visited.insert(currentState);

        if (transitions.find(currentState) != transitions.end() && transitions.at(currentState).find('e') != transitions.at(currentState).end()) {
            for (int nextState : transitions.at(currentState).at('e')) {
                if (visited.find(nextState) == visited.end() && canReachFinalState(nextState, visited)) {
                    return true;
                }
            }
        }
        return false;
    }
};

FiniteAutomaton loadAutomaton(const string &filename) {
    FiniteAutomaton automaton;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Не вдалося відкрити файл: " << filename << endl;
        exit(1);
    }

    int alphabetSize, statesSize, finalStatesSize;


    if (!(file >> alphabetSize)) {
        cerr << "Помилка читання розміру алфавіту" << endl;
        exit(1);
    }
    cout << "Алфавіт розміром: " << alphabetSize << endl;


    for (int i = 0; i < alphabetSize; i++) {
        char symbol;
        if (!(file >> symbol)) {
            cerr << "Помилка читання символу алфавіту" << endl;
            exit(1);
        }
        automaton.alphabet.insert(symbol);
        cout << "Символ алфавіту: " << symbol << endl;
    }


    if (!(file >> statesSize)) {
        cerr << "Помилка читання кількості станів" << endl;
        exit(1);
    }
    cout << "Кількість станів: " << statesSize << endl;


    for (int i = 0; i < statesSize; i++) {
        int state;
        if (!(file >> state)) {
            cerr << "Помилка читання стану" << endl;
            exit(1);
        }
        automaton.states.insert(state);
        cout << "Стан: " << state << endl;
    }


    if (!(file >> automaton.start_state)) {
        cerr << "Помилка читання початкового стану" << endl;
        exit(1);
    }
    cout << "Початковий стан: " << automaton.start_state << endl;


    if (!(file >> finalStatesSize)) {
        cerr << "Помилка читання кількості фінальних станів" << endl;
        exit(1);
    }
    cout << "Кількість фінальних станів: " << finalStatesSize << endl;


    for (int i = 0; i < finalStatesSize; i++) {
        int state;
        if (!(file >> state)) {
            cerr << "Помилка читання фінального стану" << endl;
            exit(1);
        }
        automaton.final_states.insert(state);
        cout << "Фінальний стан: " << state << endl;
    }


    int from, to;
    char input;
    while (file >> from >> input >> to) {
        automaton.addTransition(from, input, to);
        cout << "Додано перехід: " << from << " --" << input << "--> " << to << endl;
    }

    file.close();
    return automaton;
}


void generateWords(const FiniteAutomaton &automaton, const string &word, int k, vector<string> &words) {
    if (word.size() == k) {
        words.push_back(word);
        return;
    }
    for (char symbol : automaton.alphabet) {
        generateWords(automaton, word + symbol, k, words);
    }
}

bool checkAllWordsOfLengthK(const FiniteAutomaton &automaton, int k) {
    if (k == 0) {
        if (automaton.acceptsEmptyWord()) {
            cout << "Автомат сприймає всі слова довжини 0 (порожнє слово)." << endl;
            return true;
        } else {
            cout << "Автомат не сприймає всі слова довжини 0 (порожнє слово)." << endl;
            return false;
        }
    }

    vector<string> words;
    generateWords(automaton, "", k, words);

    for (const string &word : words) {
        cout << "Перевірка слова \"" << word << "\":" << endl;
        if (!automaton.accepts(word, 0, automaton.start_state)) {
            cout << "Автомат не сприймає слово: " << word << endl;
            return false;
        }
    }
    cout << "Автомат сприймає всі слова довжини " << k << endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <filename> <k>" << endl;
        return 1;
    }

    string filename = argv[1];
    int k = stoi(argv[2]);

    FiniteAutomaton automaton = loadAutomaton(filename);

    if (!checkAllWordsOfLengthK(automaton, k)) {
        cout << "Автомат не сприймає всі слова довжини " << k << endl;
    }

    return 0;
}
