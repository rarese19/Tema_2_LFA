#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

vector<char> limbaj;
vector <string> inchideri;
char sol[100];
set<string> temporar;
set<string> set_nfa;
vector<string> cuvinte_limbaj;
string temp, conditie, dfa[50][50];
int x, y, stare_f[50], str, q, mx = 0, stare_initiala = 0;
bool is_dfa = 0, is_nfa = 0, is_lm_nfa = 0;

void creare_dfa(){
    for (int i=0; i<50; i++)
        for (int j=0; j<50; j++)
            dfa[i][j] = "*";
    for (int i=0; i<50; i++)
        stare_f[i] = -1;
    ifstream f("dfa.in");
    f>>q;
    while (q!=0){
        f>>str;
        if (str == 0)
            stare_initiala = 1;
        if (str > mx)
            mx = str;
        stare_f[str]=str;
        q--;
    }
    while (f>>x){
        f>>y;
        f>>conditie;
        for (int i=0; i<conditie.length(); i++)
            if (conditie[i] == '@')
                is_lm_nfa = 1;
        dfa[x][y] = conditie;
        if (x > mx)
            mx = x;
        if (y > mx)
            mx = y;
    }
    f.close();
}

void afisare_dfa(){
    for (int i=0; i<mx+1; i++){
        for (int j=0; j<mx+1; j++)
            cout<<dfa[i][j]<<" ";
        cout<<endl;
    }
}

void creare_limbaj(){
    set<string> limbajtem;
    for (int i=0; i<mx+1; i++)
        for (int j=0; j<mx+1; j++){
            if (dfa[i][j].length() == 1 && dfa[i][j] != "*")
                limbajtem.insert(dfa[i][j]);
            if (dfa[i][j].length() > 1)
                for (int k=0; k<dfa[i][j].length(); k+=2)
                    limbajtem.insert(string(1, dfa[i][j][k]));
        }
    for (auto litera : limbajtem)
        limbaj.push_back(litera[0]);
}

void afisare_limbaj(){
    for (int sol = 0; sol < limbaj.size(); sol++)
        cout<<limbaj[sol]<<" ";
    cout<<endl;
}

void cuvinte(vector<char> limbaj, int n, int k){
    for (int i=0; i<limbaj.size(); i++){
        sol[k] = limbaj[i];
        temp = "";
        for (int l=0; l<k; l++)
            temp += sol[l];
        if (n == k)
            temporar.insert(temp);
        else
            cuvinte(limbaj, n, k+1);
    }
}

void generare(vector<char> limbaj, int n, int k){
    cuvinte_limbaj.clear();
    for (int i=1; i<=n; i++){
        cuvinte(limbaj, i, k);
    }
}

void nfa_check(){
    for (int i=0; i<mx+1; i++)
        for (int j=0; j<mx+1; j++)
            for (int k=j+1; k<mx+1; k++)
                if (dfa[i][j] != "*")
                    if (dfa[i][k] != "*")
                        for (int l = 0; l<dfa[i][j].length(); l+=2)
                            for (int o = 0; o<dfa[i][k].length(); o += 2)
                                if (dfa[i][j][l] == dfa[i][k][o])
                                    is_nfa = 1;
}

void verificare_nfa(string cuvant, int cnt, int stare ){
    for (int i=0; i<mx+1; i++){
        if (dfa[stare][i] != "*"){
            for (int l=0; l<dfa[stare][i].length(); l+=2)
                if (dfa[stare][i][l] == cuvant[cnt])
                    verificare_nfa(cuvant, cnt + 1, i);
        }
    }
    if (cnt == cuvant.length()){
        for (int i=0; i<mx+1; i++)
            if (stare == stare_f[i])
                set_nfa.insert(cuvant);
    }
}

void inch(){
    for (int i=0; i<mx+1; i++)
        inchideri.push_back("");
}

void lambda_inchideri(int baza, int stare, int v[]){
    for (int i=0; i<mx+1; i++){
        if (dfa[stare][i] != "*"){
            for (int l=0; l<dfa[stare][i].length(); l++){
                if (dfa[stare][i][l] == '@' && (v[i] == 0)){
                    v[i] = 1;
                    inchideri[baza] += (i + '0');
                    lambda_inchideri(baza, i, v);
                }
            }
        }
    }
}

void verificare_lambda_nfa(string cuvant, int cnt, int stare, vector<string> viz, bool& ok){
    if (cuvant.length() == 1){
        for (int i=0; i<mx+1; i++){
            for (int j=0; j<dfa[stare][i].length(); j++)
                if (dfa[stare][i][j] == cuvant[cnt])
                    for (int k=0; k<inchideri[i].length(); k++){
                        for (int m=0; m<mx+1; m++)
                        if ((stare_f[m] + '0') == inchideri[i][k])
                            ok = true;
                    }
        }
    }
    if(viz[stare] == cuvant.substr(cnt,cuvant.length()-1))
        return ;
    viz[stare] = cuvant.substr(cnt,cuvant.length()-1);
    for(int i=0;i<mx+1;i++)
    {
        for(int j=0;j<dfa[stare][i].size();j+=2)
        {
            if(dfa[stare][i][j] == cuvant[cnt] && dfa[stare][i][j] != '@')
                verificare_lambda_nfa(cuvant,cnt+1,i,viz,ok);
            else if(dfa[stare][i][j] == '@')
                verificare_lambda_nfa(cuvant,cnt,i,viz,ok);
        }
    }
    if(cnt == cuvant.length())
        for(int i=0;i<mx+1;i++){
            if(stare == stare_f[i])
                ok = true;
            for (int k=0; k<inchideri[stare].length(); k++){
                if ((stare_f[i] + '0') == inchideri[stare][k])
                    ok = true;
            }
        }

}



int main(){
    int lungime_cuvinte, contor = 0, choice;

    creare_dfa();

    creare_limbaj();
    for (int i=0; i<limbaj.size(); i++)
        if (limbaj[i] == '@')
            limbaj.erase(limbaj.begin() + i);

    inch();
    for (int i=0; i<mx+1; i++){
        int v[mx+1] = {0};
        lambda_inchideri(i, i, v);
    }

    nfa_check();
    //CREAREA CUVINTELOR
    system("cls");
    cout<<"Alegeti lungimea maxima: ";
    cin>>lungime_cuvinte;
    generare(limbaj, lungime_cuvinte, 0);
    for (auto sol : temporar)
            cuvinte_limbaj.push_back(sol);
    system("pause");
    while (true){
        system("cls");
        contor = 0;
        cout<<"1. Afisati limbajul\n";
        cout<<"2. Afisati matricea automatului\n";
        cout<<"3. Afisati tipul automatului\n";
        cout<<"4. Afisati cuvintele acceptate\n";
        cout<<"5. Iesire\n";
        cout<<"Introduceti alegerea: ";
        cin>>choice;
        switch (choice) {
            case 1:{
                system("cls");
                afisare_limbaj();
                system("pause");
                break;
            }
            case 2:{
                system("cls");
                afisare_dfa();
                system("pause");
                break;
            }
            case 3:{
                if (is_lm_nfa == 1){
                    system("cls");
                    cout<<"Este @-NFA.\n";
                    system("pause");
                    break;
                }
                if (is_lm_nfa == 0 && is_nfa == true){
                    system("cls");
                    cout<<"Este NFA.\n";
                    system("pause");
                    break;
                }
                if (is_lm_nfa == 0 && is_nfa == false){
                    system("cls");
                    cout<<"Este DFA.\n";
                    system("pause");
                    break;
                }
                break;
            }
            case 4:{
                system("cls");
                if (is_lm_nfa == 1){
                    if (stare_initiala == 1){
                        contor++;
                        cout<<contor<<". @"<<endl;
                    }
                    for (int i=0; i<cuvinte_limbaj.size(); i++){
                        bool ok = false;
                        vector<string> viz(mx+1,"");
                        for(int i=0;i<mx+1;i++)
                            viz[i] = "";
                        verificare_lambda_nfa(cuvinte_limbaj[i], 0, 0, viz, ok);
                        if(ok == true) {
                            contor++;
                            cout<<contor<<". "<<cuvinte_limbaj[i]<<endl;
                        }
                    }
                    cout<<"\nNUMAR TOTAL DE CUVINTE ACCEPTATE: "<<contor<<endl;
                    system("pause");
                }
                else if (is_lm_nfa == 0 && is_nfa == 1){
                    if (stare_initiala == 1){
                        contor++;
                        cout<<contor<<". @"<<endl;
                    }
                    if (set_nfa.size() == 0){
                        for (int i=0; i<cuvinte_limbaj.size(); i++)
                            verificare_nfa(cuvinte_limbaj[i], 0, 0);
                        for (auto sol : set_nfa){
                            contor++;
                            cout<<contor<<". "<<sol<<endl;
                        }
                    }
                    else {
                        for (auto sol : set_nfa){
                            contor++;
                            cout<<contor<<". "<<sol<<endl;
                        }
                    }
                    cout<<"\nNUMAR TOTAL DE CUVINTE ACCEPTATE: "<<contor<<endl;
                    system("pause");
                }
                else if (is_lm_nfa == 0 && is_nfa == 0){
                    if (stare_initiala == 1){
                        contor++;
                        cout<<contor<<". @"<<endl;
                    }
                    if (set_nfa.size() == 0){
                        for (int i=0; i<cuvinte_limbaj.size(); i++)
                            verificare_nfa(cuvinte_limbaj[i], 0, 0);
                        for (auto sol : set_nfa){
                            contor++;
                            cout<<contor<<". "<<sol<<endl;
                        }
                    }
                    else {
                        for (auto sol : set_nfa){
                            contor++;
                            cout<<contor<<". "<<sol<<endl;
                        }
                    }
                    cout<<"\nNUMAR TOTAL DE CUVINTE ACCEPTATE: "<<contor<<endl;
                    system("pause");
                }
                break;
            }
            case 5:{
                exit(0);
                break;
            }
        }
    }

    return 0;
}
