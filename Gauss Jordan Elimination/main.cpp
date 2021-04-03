#include <iostream>
#include <fstream>

using namespace std;

ifstream f("System data.txt");                                        // In date.in.c se v-a trece nr de linii, de coloane, si elementele matricei coeficientilor, si vectorul rezultatelor
// toate elementele se scriu separate prin spatiu




//Rezolvarea propriu zisa consta in 3 functii repetate la fiecare pas (init_A_next, calc_A_next, transfer_A_next)


int switch_Col(int i_piv, int j_piv, int n, int m, double ***A, int **X)          // Cauta daca se pot interschimba 2 coloane. Daca gaseste un pivot nenul pe aceeasi linie atunci interschimba si returneaza "1"
                                                                                  // altfel returneaza "0"
{
    int i, j, switchable = 0, aux_int;
    double aux_double;

    for(j = j_piv; j < m - 1; j++)
    {
        if((*A)[i_piv][j] != 0)
        {
            switchable = 1;                                                        // Daca gaseste un pivot nenul pe aceeasi linie atunci a gasit o coloana "interschimbabila"
            break;
        }
    }

    if(!switchable)
        return 0;                                                                  // Daca nu s-a gasit nici-o coloana inteschimbabila atunci iese din functie cu "0"

    for(i = 0; i < n; i++)                                                         // Daca functia inca nu s-a iesit din functie atunci coloanele se interschimba
    {
        aux_double = (*A)[i][j_piv];
        (*A)[i][j_piv] = (*A)[i][j];
        (*A)[i][j] = aux_double;
    }

    // Salvam pozitiile necunoscutelor interschimbate
    aux_int = (*X)[j];
    (*X)[j] = (*X)[j_piv];
    (*X)[j_piv] = aux_int;

    printf("Matrix if we switch %d cu %d\n",j_piv+1,j+1);
    cout<<endl;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            cout<<" "<<(*A)[i][j];
        }
        cout<<endl;
    }
    cout<<"\n\n";
    return 1;

}



int init_A_next(int n, int m, int i_piv, int j_piv, double ***A, double ***A_next, int **X)
{
    int i, j;

    printf("\npiv=%.2f\n",(*A)[i_piv][j_piv]);

    if((*A)[i_piv][j_piv] == 0)
    {
        if(!switch_Col(i_piv, j_piv, n, m, A, X))
            return 1;                                                   // Daca pivotul este 0 si nu este posibil sa se gaseasca vreo coloana bune de interschimbare
    }                                                                   // Atunci se returneaza "1"


    // Dupa terminarea posibilelor operatiuni de switch intre coloane, se pregateste A_next pentru etapa de calcul


    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {

            if((*A)[i][j] == 0)
            {
                (*A_next)[i][j] = 0;                               // Elementele care sunt 0 in A vor fi 0 si in A_next
            }

            else if(i_piv == i)
            {
                (*A_next)[i][j] = (*A)[i][j];                         // Se copiaza elementele de pe linia pivotului
            }

            else if(j_piv == j)
            {
                (*A_next)[i][j] = 0;                               // Elementele de pe coloana pivotului devin 0
            }
        }
        cout<<endl;
    }

    return 0;
}

void calc_A_next(int n, int m, int i_piv, int j_piv, double ***A, double ***A_next)            // Aici se vor calcula restul elementelor din A_next folosind regula triunghiului
{
    double piv = (*A_next)[i_piv][j_piv];
    int i,j;

    printf("\npiv:%.2f\n",piv);                                 // Se afiseaza piv la stadiul de calcul(trebuie sa fie egal cu cel de la stadiul de pregatire)

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(i != i_piv && j != j_piv)                                                                                    // Regula dreptunghiului se va folosi doar pe elementele care nu se afla pe aceeasi linie sau coloana cu piv
            {
                printf("\nA_next[%d][%d]= (%.2f * %.2f) - (%.2f * %.2f)\n",i+1,j+1,piv,(*A)[i][j],(*A)[i_piv][j],(*A)[i][j_piv]);
                (*A_next)[i][j] = (piv * ((*A)[i][j])) - ((*A)[i_piv][j]) * ((*A)[i][j_piv]);                                               // A_next va lua elementele prelucrate prin regula dreptunghiului din A
                printf("A_next[%d][%d] becomes %.2f\n\n",i+1,j+1,(*A_next)[i][j]);

            }
        }
    }
}

void transfer_A_next(int n, int m, double ***A, double ***A_next)                             // Functia este folosita la finalul pasului de procesare. Transfera elementele prelucrate din A_next in A
{
    int i, j;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            (*A)[i][j] = (*A_next)[i][j];
        }
    }
}

bool incompatible_Syst(int n, int m, double ***A_next)                           // Functia va fi folosita in sol_Sist() si va returna 1 daca sist este incompatibil si 0 daca este compatibil
{
    int i, j, null_coef;

    for(i = 0; i < n; i++)
    {
        null_coef = 1;
        for(j = 0; j < m - 1; j++)
        {
            if((*A_next)[i][j] != 0)
            {
                null_coef = 0;                                 // Toate elementele de pe o linie care nu se afla pe ultima coloana vor fi coeficientii necunoscutelor
            }

        }

        if(null_coef && ((*A_next)[i][m-1] != 0))                // Daca pe o linie coef sunt nuli si ultimul element care corespunde rezultatelor este nenul atunci
            {
                printf("%.2f = %.2f",(*A_next)[i][j], (*A_next)[i][m-1]);
                return 1;
            }

                                                 // se ajunge la o contradictie de tipu "0 = ceva" ceea ce este fals si atunci sistemul este incompatibil
    }

    return 0;                                                 // Altfel daca functia a ajuns cu succes la final atunci sistemul este compatibil

}

// Aceasta functie este apelata dupa ce s-au terminat pasii de prelucrare. Are dublu scop: de a determina tipul sistemului si de afisa solutiile

void sol_Syst(int n, int m, double ***A_next, int **X)
{
    int i, j, new_n = n, undet_degree = 0;
    bool empty_line = 1;
    // Daca dupa procesare raman linii goale trebuie sa le scadem numarul de linii. Aceasta valoare va fi salvata in n_nou
    for(i = 0; i < n; i++)
    {
        empty_line = 1;
        for(j = 0; j < m; j++)
        {

            if((*A_next)[i][j] != 0)
            {
                empty_line = 0;
                break;
            }
        }

        if(empty_line)
            new_n--;
    }

    if(incompatible_Syst(new_n,m,A_next))                                              // Daca sistemul este incompatibil atunci programul va fi terminat
    {
        printf("\n\n System is incompatible \n\n");
        exit(0);
    }


    printf("\nSolutions are: \n");
    // Aici se afla solutiile in urmatorul mod:
    // La fiecare linie se va afla o necunoscuta (principala)=(A_next[i][i]) care va fi egala cu ("ultimul element de pe linie" - ("suma celorlaltor necunoscute"))/ A_next[i][i]
    for(i = 0; i < new_n; i++)
    {
        printf("x%d = %.2f ", (*X)[i]+1, ((*A_next)[i][m-1]) / ((*A_next)[i][i]));
        for(j = 0; j < m - 1; j++)
            if(((j < i) || (j > i)) && ((*A_next)[i][j]))                            // Nu se vor lua in calcul necunoscuta principala de pe linia respectiva si elementele nule
                printf("+ %.2fx%d ",(-1)*(((*A_next)[i][j])/((*A_next)[i][i])),(*X)[j]+1);
        cout<<endl;

    }
    // Aici se determina tipul sistemului si daca e nedeterminat se aflta si ordinul de nedeterminare

    if(i >= m-1)                                                                // Daca la finalul prelucrarii si dupa eliminarea liniilor nule matricea ramasa este patratica atunci sistemul este compatibil determinat
        printf("The system is determined compatible");
    else                                                                        // Altfel sistemul este compatibil nedeterminat iar coloanele ramase corespund necunoscutelor secundare
    {

        while(i < m-1)
        {
            printf("x%d ",(*X)[i]+1);
            i++;
            undet_degree++;

        }
        printf(" are secondary solution and belong to R\nSystem is undetermined compatible of %d degree\n\n", undet_degree);     // Nr de necunoscute secundare reprezinta ordinul de nedeterminare
    }
}

int main()
{
    double **A_next, **A, *B;                           // Vom avea nevoie de A(matricea initiala) si A_next (matricea rezultata in urma calculelor)
    int *X;
    int n, m, i, j, k = 0, min_row_col, End;
    //Citim din fisier nr de linii, nr de coloane, matr A si vectorul de rezultate B
    f>>n;
    f>>m;



    // Alocari dinamice
    X = (int *) malloc(m * sizeof(int));

    B = (double *) malloc(n * sizeof(double));

    A = (double **) malloc(n*sizeof(double *));
    for(i = 0; i < n; i++)
        A[i] = (double *) malloc( (m + 1) * sizeof(double) );

    A_next = (double **) malloc(n*sizeof(double *));
    for(i = 0; i < n; i++)
        A_next[i] = (double *) malloc( (m + 1) * sizeof(double) );




    if(X == NULL || A == NULL || B == NULL || A_next == NULL)
    {
        cout << "\nERROR! Memory was not properly allocated \n";
        exit(0);
    }



    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            f>>A[i][j];
        }
    }


    for(i = 0; i < n; i++)
    {
        f>>B[i];
    }
    // Afisam Matricea A si vectorul B
    printf("Coef matrix:\n");
    cout<<endl;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            cout<<" "<<A[i][j];
        }
        cout<<endl;
    }

    cout<<endl;
    printf("Results vector:\n");
    cout<<endl;

    for(i = 0; i < n; i++)
    {
        cout <<B[i]<<endl;
    }

    // Se initializeaza vectorul coloanelor
    for(i = 0; i < m; i++)
        X[i] = i;

    // Extindem cu o coloana matricea A si pe acea coloana se copiaza B
    cout<<"\nExtending matrix A with vector B\n\n";

    for(i = 0; i < n; i++)
        A[i][m] = B[i];
    m++;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            cout<<" "<<A[i][j];
        }
        cout<<endl;
    }



    // Se initializeaza matricea A_next cu 9999 pt a fi mai usor de observat modificarile de la prelucrare
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            A_next[i][j] = 9999;
        }
    }

    cout<<"\n Initializing A_next with 9999\n";
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            cout<< " "<<A_next[i][j];
        }
        cout<<endl;
    }
    // Avem nevoie de min dinte nr de linii si de coloane deoarece acesta va dicta nr max de pivoti pe care ii putem avea
    min_row_col = n;
    if(min_row_col > m)
        min_row_col = m;

    // PRELUCRAREA EFECTIVA A MATRICILOR

    k = 0;
    do                                                                                  // Se vor repeta cele 3 functii de prelucrare pana se atinge nr max de pivoti sau pana ajunge piv 0
    {
        cout<<"\n Current step "<<k+1<<endl;

        End = init_A_next(n, m, k, k, &A, &A_next, &X);                                                     //  Aceasta secventa are dublu scop: va apela functia de initializare a lui A_next, si in cazul in care se alege piv nul, se va opri prelucrarea
        if(End)
        {
            cout<<"\nSTOP\n";
            break;
        }

        cout<<"\n A_next initialized\n";                                               // Afiseaza A_next dupa initializare
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < m; j++)
            {
                cout<< " "<<A_next[i][j];
            }
            cout<<endl;
        }


        calc_A_next(n, m, k, k, &A, &A_next);                                                         // Se apeleaza functia de calcul prin regula dreptunghiului

        cout<<"\n A_next calculated\n";                                                // Afiseaza A_next dupa prelucrare
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < m; j++)
            {
                cout<< " "<<A_next[i][j];
            }
            cout<<endl;
        }

        transfer_A_next(n, m, &A, &A_next);                                                        // La finalul prelucrarii se transfera A_next in A

        k++;
    }
    while(k < min_row_col);


    sol_Syst(n, m, &A_next, &X);                                                                   // Apelam functia care va afisa solutiile si tipul sistemului

    free(X);

    free(B);

    for(i = 0; i < n; i++)
        free(A[i]);
    free(A);


    for(i = 0; i < n; i++)
        free(A_next[i]);
    free(A_next);

    return 0;
}
