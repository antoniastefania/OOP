#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <memory>

using namespace std;


// Clasa de baza abstracta pentru pasi
class Step
{
public:
    virtual void execute() = 0;
    virtual string getStepType() const = 0;
    virtual string getDescription() const = 0;
    virtual void writeDetailsToFile(ofstream& file) const = 0;
    virtual ~Step() {}
    virtual bool isNumberInputStep() const { return false; }
};
// Clasa pentru pasul de tip titlu
class TitleStep : public Step
{
private:
    string title;
    string subtitle;

public:
    TitleStep(const string& title, const string& subtitle)
        : title(title), subtitle(subtitle) {}
    void execute() override
    {

        cout << "TITLE: ";
        cin.ignore();
        getline(cin, title);

        cout << "SUBTITLE: ";
        getline(cin, subtitle);
    }


    std::string getStepType() const override
    {
        return "Title Step";
    }

    std::string getDescription() const override
    {
        return title + " - " + subtitle;
    }

    void writeDetailsToFile(ofstream &file) const override
{
    file << "TITLE Step" << endl;
    file << "Title: " << title << endl;
    file << "Subtitle: " << subtitle << endl;
    file << endl;
}


};

// Clasa pentru pasul de tip text
class TextStep : public Step
{
private:
    string title;
    string text;

public:
    TextStep(const string& title, const string& text)
        : title(title), text(text) {}

     void execute() override
    {

        cout << "TITLE: ";
        cin.ignore();
        getline(cin, title);

        cout << "COPY: ";
        getline(cin, text);
    }

    std::string getStepType() const override
    {
        return "Text Step";
    }

    std::string getDescription() const override
    {
        return title + " - " + text;
    }

    void writeDetailsToFile(ofstream &file) const override
{
    file << "TEXT Step" << endl;
    file << "Title: " << title << endl;
    file << "Text: " << text << endl;
    file << endl;
}


};




// Clasa pentru pasul de tip text input
class TextInputStep : public Step
{
private:
    string description;
    string textInput;

public:
    TextInputStep(const string& desc, const string& textInput) : description(desc), textInput(textInput) {}

    void execute() override
    {

        cout << "DESCRIPTION: ";
        cin.ignore();
        getline(cin, description);

        cout << "TEXT INPUT: ";
        getline(cin, textInput);


        if (textInput.empty())
        {
            throw runtime_error("Invalid input. Text input cannot be empty.");
        }
    }
    std::string getStepType() const override
    {
        return "Text Input Step";
    }

    std::string getDescription() const override
    {
        return description + " - " + textInput;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "TEXT INPUT Step" << endl;
    file << "Description: " << description << endl;
    file << "Text Input: " << textInput << endl;
    file << endl;
}
};


class NumberInputStep : public Step
{
private:
    string description;
    float numberInput;
    bool executed;

public:
    NumberInputStep(const string& desc) : description(desc), numberInput(0.0f), executed(false) {}

    float getNumber() const
    {
        return numberInput;
    }

    bool isExecuted() const {
        return executed;
    }

    void execute() override {
        // Implementation for NUMBER INPUT step
        std::cout << "Introduceti o descriere: " << description << std::endl;

        while (true)
        {
            std::cout << "Introduceti un numar: ";
            if (std::cin >> numberInput)
            {
                // Verifica daca input-ul este valid
                break;
            }
            else
            {
                // Clear errorul de pe cin si ignora restul input-ului invalid
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Input invalid. Va rugam introduceti un numar valid." << std::endl;
            }
        }


        executed = true;
    }

    std::string getStepType() const override
    {
        return "Number Input Step";
    }

    std::string getDescription() const override
    {
        return description + " - " + std::to_string(numberInput);
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "NUMBER INPUT Step" << endl;
    file << "Description: " << description << endl;
    file << "Number Input: " << numberInput << endl;
    file << endl;  // Adaugă o linie goală între detalii
}
};


// Clasa pentru pasul de tip calculus

class CalculusStep : public Step
{
private:
    int steps;
    string operation;
    vector<NumberInputStep*> inputSteps;
    float result;

public:
   CalculusStep(int s, const string& op) : steps(s), operation(op), result(0.0f) {}


    void addInputStep(NumberInputStep* step)
    {
        inputSteps.push_back(step);
    }

    void execute()
    {
        // Verificarea efectuării pasului NumberInputStep
        for (const auto& inputStep : inputSteps)
        {
            if (!inputStep->isExecuted())
            {
                cout << "Eroare: Trebuie executat NumberInputStep înainte de CalculusStep!" << endl;
                return;
            }
        }

        // Extragerea numărului de la pasul NumberInputStep
        float userInput = inputSteps[0]->getNumber();

        // Solicitarea utilizatorului pentru introducerea unui alt număr
        cout << "Introduceți un număr pentru operații: ";
        float userSecondInput;
        cin >> userSecondInput;

        // Afisarea meniului de operații
        cout << "Alegeti o optiune:\n";
        cout << "1. Adunare (+)\n";
        cout << "2. Scădere (-)\n";
        cout << "3. Înmulțire (*)\n";
        cout << "4. Împărțire (/)\n";
        cout << "5. Minim (min)\n";
        cout << "6. Maxim (max)\n";
        cout << "Introduceti numarul corespunzator operatiei pe care vreti sa o alegeti: ";

        int operationChoice;
        cin >> operationChoice;
        cin.ignore();

        // Efectuarea calculelor in functie de operatia aleasa
        switch (operationChoice)
        {
        case 1:
            result = userInput + userSecondInput;
            break;
        case 2:
            result = userInput - userSecondInput;
            break;
        case 3:
            result = userInput * userSecondInput;
            break;
        case 4:
            // Verificarea împărțirii la zero
            if (userSecondInput != 0)
            {
                result = userInput / userSecondInput;
            }
            else
            {
                cout << "Eroare: Impartirea la zero nu este posibila. Va rugam sa incercati din nou." << endl;
                return;
            }
            break;
        case 5:
            result = min(userInput, userSecondInput);
            break;
        case 6:
            result = max(userInput, userSecondInput);
            break;
        default:
            cout << "Eroare: Operatie necunoscuta." << endl;
            return;
        }

        cout << "Rezultat: " << result << endl;

        char continueChoice;
        cout << "Vrei sa efectuezi alte operatii? (d/n): ";
        cin >> continueChoice;

        if (continueChoice == 'd' || continueChoice == 'D')
        {
            execute(); // Reapelarea functiei pentru a efectua alte operatii cu aceleasi numere
        }
    }

    std::string getStepType() const override
    {
        return "Calculus Step";
    }

    std::string getDescription() const override
    {

        return std::to_string(steps) + " steps - " + operation;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "CALCULUS Step" << endl;
    file << "Result: " << result << endl;
    file << endl;
}
};



// Clasa pentru pasul de tip TEXT FILE input
class TextFileInputStep : public Step
{
private:
    std::string description;
    std::string fileName;

public:
    TextFileInputStep(const std::string& desc, const std::string& file) : description(desc), fileName(file) {}

    void execute() override
    {
        std::cout << "Introduceti o descriere: " << description << std::endl;
        std::cout << "Introduceti numele fisierului .txt: ";
        std::cin >> fileName;

        std::ifstream fileStream(fileName);
        std::string fileContent;
        if (fileStream.is_open())
        {
            std::string line;
            while (getline(fileStream, line))
            {
                fileContent += line + "\n";
            }
            fileStream.close();

            std::cout << "Continutul fisierului:\n" << fileContent << std::endl;
        }
        else
        {
            std::cerr << "Eroare: Fisierul nu a putut fi deschis." << std::endl;
        }
    }

    std::string getStepType() const override
    {
        return "Text File Input Step";
    }

    std::string getDescription() const override
    {
        return description + " - " + fileName;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "TEXT FILE INPUT Step" << endl;
    file << "File: " << fileName << endl;
    file << "Description: " << description << endl;
    file << endl;
}
};

// Clasa pentru pasul de tip CSV FILE input
class CSVFileInputStep : public Step
{
private:
    std::string description;
    std::string fileName;

public:
    CSVFileInputStep(const std::string& desc, const std::string& file) : description(desc), fileName(file) {}

    void execute() override
    {
        std::cout << "Introduceti o descriere: " << description << std::endl;
        std::cout << "Introduceti numele fisierului .csv: ";
        std::cin >> fileName;

        std::ifstream fileStream(fileName);
        std::string fileContent;
        if (fileStream.is_open())
        {
            std::string line;
            while (getline(fileStream, line))
            {
                fileContent += line + "\n";
            }
            fileStream.close();

            std::cout << "Continutul fisierului:\n" << fileContent << std::endl;
        }
        else
        {
            std::cerr << "Eroare: Fisierul nu a putut fi deschis." << std::endl;
        }
    }

     std::string getStepType() const override
    {
        return "CSV File Input Step";
    }

    std::string getDescription() const override
    {
        return description + " - " + fileName;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "CSV FILE INPUT Step" << endl;
    file << "File name: " << fileName << endl;
    file << "Description: " << description << endl;
    file << endl;
}

};

// Clasa pentru pasul DISPLAY
class DisplayStep : public Step
{
private:
    int step;
    string content;
    string fileName;

public:
    DisplayStep(int s, const string& c, const string& file) : step(s), content(c), fileName(file) {}


    void execute()
    {
        cout << "Executarea pasului DISPLAY pentru TEXT FILE (tastati 1) sau pentru CSV FILE (tastati 2) : " << step << endl;

        int fileTypeChoice;
        cout << "Selectati tipul fisierului pentru afisare (1 - TEXT FILE, 2 - CSV FILE): ";
        cin >> fileTypeChoice;

        ifstream inputFile(fileName);

        if (!inputFile)
        {
            throw runtime_error("Eroare la deschiderea fisierului " + fileName);
        }

        if (fileTypeChoice == 1)
        {
            // Afișare conținut TEXT FILE
            string line;
            while (getline(inputFile, line))
            {
                cout << line << endl;
            }
        }
        else if (fileTypeChoice == 2)
        {
            // Afișare conținut CSV FILE
            string csvLine;
            while (getline(inputFile, csvLine))
            {
                cout << csvLine << endl;
            }
        }
        else
        {
            throw runtime_error("Selectie invalida a tipului de fisier.");
        }

        inputFile.close();
    }
     std::string getStepType() const override
    {
        return "Display Step";
    }

    std::string getDescription() const override
    {
        return content + " - " + fileName;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "DISPLAY Step" << endl;
    file << "file name: " << fileName << endl;
    file << "Content: " << content << endl;
    file << endl;  // Adaugă o linie goală între detalii
}

};

// Clasa pentru pasul de tip OUTPUT
class OutputStep : public Step
{
private:
    int stepNumber;
    std::string fileName;
    std::string title;
    std::string description;

public:
    OutputStep(int step, const std::string& file, const std::string& t, const std::string& desc)
        : stepNumber(step), fileName(file), title(t), description(desc) {}

    void execute() override
    {
        std::cout << "Introduceti numele fisierului de iesire .txt: ";
        std::cin >> fileName;

        std::ofstream fileStream(fileName);
        if (fileStream.is_open())
        {
            fileStream << "Title: " << title << "\n";
            fileStream << "Description: " << description << "\n";
            fileStream << "Step Number: " << stepNumber << "\n";
            fileStream.close();

            std::cout << "Fisierul de iesire a fost generat cu succes." << std::endl;
        }
        else
        {
            std::cerr << "Eroare: Fisierul de iesire nu a putut fi creat." << std::endl;
        }
    }


    std::string getStepType() const override
    {
        return "Output Step";
    }

    std::string getDescription() const override
    {
        return std::to_string(stepNumber) + " - " + fileName + " - " + title + " - " + description;
    }
    void writeDetailsToFile(ofstream &file) const override
{
    file << "OUTPUT Step" << endl;
    file << "Title: " << title << endl;
    file << "Text: " << description << endl;
    file << endl;
}

};


class Flow
{
private:
    string name;
    vector<Step*> steps;
    time_t creationTime;
    int startCount;  // Numărul de porniri ale procesului
    int completionCount;  // Numărul de finalizări ale procesului
    vector<int> skippedScreens;  // Vector pentru a ține evidența ecranelor sărite
    vector<int> errorScreens;  // Vector pentru a ține evidența ecranelor de eroare
    float totalErrors;  // Numărul total de erori pentru analiza medie
    bool isCompleted;  // Flag pentru a verifica dacă procesul a fost finalizat

public:
    Flow(const string& n) : name(n), startCount(0), completionCount(0), totalErrors(0), isCompleted(false)
    {
        creationTime = time(nullptr);
    }
    void addStep(Step* step)
    {
        steps.push_back(step);
    }
    const vector<Step*>& getSteps() const
    {
        return steps;
    }


    void run()
    {
        startCount++;
        for (Step* step : steps)
        {
            if (!isCompletedSuccessfully())
            {
                step->execute();
            }
        }
        completionCount++;
        isCompleted = true;
    }

    void analyze() const
    {
        cout << "Analiza procesului " << name << ":\n";
        cout << "  - Numarul de porniri: " << startCount << endl;
        cout << "  - Numarul de finalizari: " << completionCount << endl;

        if (completionCount > 0)
        {
            float averageErrors = totalErrors / completionCount;
            cout << "  - Numarul mediu de erori per proces finalizat: " << averageErrors << endl;
        }
        else
        {
            cout << "  - Procesul nu a fost finalizat niciodata.\n";
        }

        cout << "  - Numarul total de ecrane sarite: " << skippedScreens.size() << endl;
        cout << "  - Numarul total de ecrane de eroare: " << errorScreens.size() << endl;

        // Alte informații de analiză pot fi adăugate aici
    }

    void displayCreationTime() const
    {
        cout << "Procesul " << name << " a fost creat la: " << put_time(localtime(&creationTime), "%Y-%m-%d %H:%M:%S") << endl;
    }

    void markScreenSkipped(int screenNumber)
    {
        skippedScreens.push_back(screenNumber);
    }

    void markScreenError(int screenNumber)
    {
        errorScreens.push_back(screenNumber);
        totalErrors++;
    }

    bool isCompletedSuccessfully() const
    {
        return isCompleted;
    }

    ~Flow()
    {
        for (Step* step : steps)
        {
            delete step;
        }
        steps.clear();
    }

    string getName() const
    {
        return name;
    }

   string getStepsInfo() const
{
    string stepsInfo;
    for (const Step* step : steps)
    {
        stepsInfo += step->getDescription() + " | ";
    }
    return stepsInfo;
}


};

class FlowManager
{
private:
    vector<Flow*> flows;

public:
    const vector<Flow*>& getFlows() const
    {
        return flows;
    }

    void createFlow(const string& name)
    {
        Flow* newFlow = new Flow(name);
        flows.push_back(newFlow);
    }

    void displayAvailableSteps()
    {
        cout << "Available steps:\n";
        cout << "1. Title Step\n";
        cout << "2. Text Step\n";
        cout << "3. Text Input Step\n";
        cout << "4. Number Input Step\n";
        cout << "5. Calculus Step\n";
        cout << "6. Display Step\n";
        cout << "7. Text File Input Step\n";
        cout << "8. CSV File Input Step\n";
        cout << "9. Output Step\n";
        cout << "10. End Step\n";
    }

    void addStepToFlow(Flow* flow, Step* step)
    {

        flow->addStep(step);
        if (!flow->isCompletedSuccessfully())
        {
            step->execute();
        }
    }

    void runFlow(Flow* flow)
    {
        flow->run();
    }

    void deleteFlow(Flow* flow)
    {
        auto it = find(flows.begin(), flows.end(), flow);
        if (it != flows.end())
        {
            flows.erase(it);
            delete flow;
        }
    }

    Flow* getFlowByName(const string& name)
    {
        auto it = find_if(flows.begin(), flows.end(),
                          [name](const Flow* flow)
        {
            return flow->getName() == name;
        });

        return (it != flows.end()) ? *it : nullptr;
    }

    void analyzeFlow(Flow* flow) const
    {
        flow->analyze();
    }

    ~FlowManager()
    {
        for (Flow* flow : flows)
        {
            delete flow;
        }
        flows.clear();
    }

    void addFlow(Flow* flow)
{
    flows.push_back(flow);
}

    void saveFlowsToFile(const string& filename) const
    {
        ofstream outputFile(filename);

        if (!outputFile.is_open())
        {
            cerr << "Eroare la deschiderea fisierului " << filename << endl;
            return;
        }

        for (const Flow* flow : flows)
        {
            outputFile << "Numele procesului: " << flow->getName() << "\n";

            // Adăugați detaliile fiecărui pas în fișier
            for (const Step* step : flow->getSteps())
            {
                step->writeDetailsToFile(outputFile);
            }

            outputFile << "-------------------------\n";
        }

        outputFile.close();
    }


    void displayFlowsFromFile(const string& filename) const
    {
        ifstream inputFile(filename);

        if (!inputFile.is_open())
        {
            cerr << "Eroare la deschiderea fisierului " << filename << endl;
            return;
        }

        string line;
        while (getline(inputFile, line))
        {
            cout << line << endl;
        }

        inputFile.close();
    }




};



// Clasa pentru pasul de tip final
class EndStep : public Step
{
public:
    void execute() override
    {
        cout << "Sfarsitul procesului. Procesul a fost completat cu succes." << endl;
    }
};



FlowManager flowManager;

int main()
{

        vector<shared_ptr<Step>> steps;

    cout << "---------------------------------------" << endl;
    cout << "           BINE ATI VENIT!             " << endl;
    cout << "---------------------------------------" << endl;


    int option;


    try
    {
        ofstream outputFile("procese.txt", ios::app);  // Deschide fisierul o singura data pentru adaugarea pasilor.

        while (true)
        {
            cout << "\nAlegeti o optiune:\n";
            cout << "1. Creati un proces nou\n";
            cout << "2. Afisati numele proceselor existente\n";
            cout << "3. Rulati un proces\n";
            cout << "4. Stergeti un proces\n";
            cout << "5. Afisati detalii despre un proces\n";
            cout << "6. Analizati un proces\n";
            cout << "0. Iesire\n";
            cout << "Optiune: ";
            cin >> option;

            switch (option)
            {

            case 0:
                {
                    cout << "La revedere!\n";
                    return 0;
                }
            case 1:
                {
                    string flowName;
                    cout << "Introduceti numele noului proces: ";
                    cin >> flowName;


                    outputFile << "Nume proces: " << flowName << "\n";  // Salveaza numele procesului



                    Flow* newFlow = new Flow(flowName);
                    flowManager.createFlow(flowName);
                    flowManager.displayAvailableSteps();

                    int stepOption;

                while (true)
                {
                    cout << "Alegeti un pas (tasta 10 pentru a finaliza): ";
                    cin >> stepOption;

                        if (stepOption == 10)
                        {

                            // Adăugarea informațiilor despre pași în fișier la finalizarea procesului
                            ofstream outputFile("procese.txt", ios::app);
                            outputFile << newFlow->getName() << " - " << newFlow->getStepsInfo() << "\n";
                            outputFile << endl;  // Adauga un newline dupa terminarea pasilor
                            outputFile.close();
                            break;
                        }
                        // Verificare dacă input-ul este un număr între 1 și 10
                        while (cin.fail() || stepOption < 1 || stepOption > 10)
                        {
                            // Curățăm starea de eroare și ignorăm restul input-ului invalid
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            // Afișăm mesaj de eroare și reluăm citirea
                            cout << "Optiune invalida. Va rugam sa reintroduceti optiunea (intre 1 si 10): ";
                            cin >> stepOption;
                        }

                    Step* selectedStep = nullptr;
                    switch (stepOption)
                    {
                        case 1:
                            {
                                TitleStep selectedStep("", "");
                                selectedStep.execute();;
                                break;
                            }
                        case 2:
                            {
                                TextStep selectedStep("", "");
                                selectedStep.execute();;
                                break;
                            }

                        case 3:
                            {
                                TextInputStep selectedStep("", "");
                                selectedStep.execute();;
                                break;
                            }
                        case 4:
                            {

                                // Adăugați un NumberInputStep
                                string description;
                                cout << "Introduceti o descriere pentru input: ";
                                cin.ignore();
                                getline(cin, description);


                                NumberInputStep* numberInputStep = new NumberInputStep(description);

                                // Executați pasul și verificați dacă a fost executat cu succes
                                numberInputStep->execute();
                                if (numberInputStep->isExecuted())
                                {
                                    selectedStep = numberInputStep;
                                }
                                else
                                {

                                    delete numberInputStep;
                                    selectedStep = nullptr;
                                }

                                break;
                            }

                        case 5:
                            {


                                // Adăugați un CalculusStep
                                int steps;
                                string operation;
                                cout << "Introduceti numarul de pasi pentru CalculusStep: ";
                                cin >> steps;
                                cout << "Introduceti operatia pentru CalculusStep: ";
                                cin.ignore();
                                getline(cin, operation);
                                CalculusStep* calculusStep = new CalculusStep(steps, operation);

                                // Adăugăm input-uri pentru CalculusStep
                                for (int i = 0; i < steps; ++i)
                                {
                                    cout << "Adaugati input pentru pasul " << i + 1 << endl;
                                    NumberInputStep* inputStep = new NumberInputStep("Descriere");
                                    flowManager.addStepToFlow(newFlow, inputStep);
                                    calculusStep->addInputStep(inputStep);
                                    // Nu este necesar să rulezi flow-ul aici
                                }

                                selectedStep = calculusStep;
                                break;
                            }




                        case 6:
                            {
                                // Adăugați un DisplayStep
                                int step;
                                string content, fileName;
                                cout << "Introduceti numarul pasului pentru DisplayStep: ";
                                cin >> step;
                                cout << "Introduceti continutul pentru DisplayStep: ";
                                cin.ignore();
                                getline(cin, content);
                                cout << "Introduceti numele fisierului pentru DisplayStep: ";
                                getline(cin, fileName);
                                selectedStep = new DisplayStep(step, content, fileName);
                                break;
                            }

                        case 7:
                            {
                                // Adăugați un TextFileInputStep
                                string description, fileName;
                                cout << "Introduceti descrierea pentru TextFileInputStep: ";
                                cin.ignore();
                                getline(cin, description);
                                cout << "Introduceti numele fisierului pentru TextFileInputStep: ";
                                getline(cin, fileName);
                                selectedStep = new TextFileInputStep(description, fileName);
                                break;
                            }

                        case 8:
                            {
                                // Adăugați un CsvFileInputStep
                                string description, fileName;
                                cout << "Introduceti descrierea pentru CsvFileInputStep: ";
                                cin.ignore();
                                getline(cin, description);
                                cout << "Introduceti numele fisierului pentru CsvFileInputStep: ";
                                getline(cin, fileName);
                                selectedStep = new CSVFileInputStep(description, fileName);
                                break;
                            }

                        case 9:
                            {
                                // Adăugați un OutputStep
                                int step;
                                string fileName, title, description;
                                cout << "Introduceti numarul pasului pentru OutputStep: ";
                                cin >> step;
                                cout << "Introduceti numele fisierului pentru OutputStep: ";
                                cin.ignore();
                                getline(cin, fileName);
                                cout << "Introduceti titlul pentru OutputStep: ";
                                getline(cin, title);
                                cout << "Introduceti descrierea pentru OutputStep: ";
                                getline(cin, description);

                                selectedStep = new OutputStep(step, fileName, title, description);
                                break;
                            }
                        }

                        if (selectedStep)
                        {
                            // Adaugarea pasului la flow
                            flowManager.addStepToFlow(newFlow, selectedStep);
                            newFlow->run();  // Executăm pasul adăugat imediat
                        }
                        cout << "Alegeti urmatorul pas sau tasta 10 pentru a finaliza: ";
                    }

                // Adăugăm procesul finalizat în manager
                flowManager.addFlow(newFlow);
                cout << "Procesul " << flowName << " a fost creat și finalizat cu succes!" << endl;
                // Salvare procese în fișier
                flowManager.saveFlowsToFile("procese.txt");


                if (newFlow != nullptr)
        {
            outputFile << newFlow->getStepsInfo() << "\n";
            outputFile << endl;  // Adauga un newline dupa terminarea pasilor
        }
                break;
            }
            case 2:
            {
                cout << "Numele proceselor existente:\n";
                for (const Flow* flow : flowManager.getFlows())
                {
                    cout << "- " << flow->getName() << endl;
                }

                // Afișare procese din fișier
                flowManager.displayFlowsFromFile("procese.txt");
                break;
            }
            case 3:
            {
                string flowName;
                cout << "Introduceti numele procesului pe care doriti sa-l rulati: ";
                cin >> flowName;
                    Flow* selectedFlow = flowManager.getFlowByName(flowName);
                    if (selectedFlow)
                    {
                        cout << "Procesul: " << selectedFlow->getName() << "\n";
                        cout << "Pasi selectati: " << selectedFlow->getStepsInfo() << "\n";
                        flowManager.runFlow(selectedFlow);
                    }
                    else
                    {
        cout << "Procesul cu numele " << flowName << " nu exista!" << endl;
    }
    break;
            }
            case 4:
            {
                string flowName;
                cout << "Introduceti numele procesului pe care doriti sa-l stergeti: ";
                cin >> flowName;
                Flow* selectedFlow = flowManager.getFlowByName(flowName);
                if (selectedFlow)
                {
                    flowManager.deleteFlow(selectedFlow);
                    cout << "Procesul " << flowName << " a fost sters cu succes!" << endl;
                }
                else
                {
                    cout << "Procesul cu numele " << flowName << " nu exista!" << endl;
                }
                break;
            }
            case 5:
            {
                string flowName;
                cout << "Introduceti numele procesului pentru detalii: ";
                cin >> flowName;
                Flow* selectedFlow = flowManager.getFlowByName(flowName);
                if (selectedFlow)
                {
                    selectedFlow->displayCreationTime();
                }
                else
                {
                    cout << "Procesul cu numele " << flowName << " nu exista!" << endl;
                }
                break;
            }
            case 6:
            {
                string flowName;
                cout << "Introduceti numele procesului pentru analiza: ";
                cin >> flowName;
                Flow* selectedFlow = flowManager.getFlowByName(flowName);
                if (selectedFlow)
                {
                    flowManager.analyzeFlow(selectedFlow);
                }
                else
                {
                    cout << "Procesul cu numele " << flowName << " nu exista!" << endl;
                }
                break;
            }

            default:
                cout << "Optiune invalida. Va rugam sa reintroduceti optiunea." << endl;
            }
        }
        outputFile.close();  // Inchide fisierul dupa ce ai terminat cu toate operatiunile
    }
    catch (const exception& e)
    {
        cerr << "Eroare: " << e.what() << endl;
    }


    return 0;

}
