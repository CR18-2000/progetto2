#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


using namespace std;
void printknapSack(int W, int wt[], int val[], int n);
double velocita(double vmax, double vmin, int W, int C);
double tempo_arco(int dis, double vel);

vector<int> zaino;
vector<int> citta_contr;


int main() {

    int N, S;   //N e il numero di citta, S la sorgente e il punto di arrivo
    int M, C;   //M e il numero di pietre, C e la capacita del guanto
    double R, vmin, vmax;   //R e l'energia usata per unita di tempo, vmin e vmax le velocita max e min
    ifstream in ("input.txt");
    in >> N >> S;
    in >> M >> C >> R >> vmin >> vmax;

    int pesi_pietre[M];
    int energia_pietre[M];
    int citta[M];   //citta dove prendere la pietra
    int num_citta[M];       //numero di citta in cui e' presente la pietra
    vector<int> location[M];    //luoghi dove sono le pietre;
      
    //leggere pesi e energia delle pietre
    for (int i=0; i<M; i++) {
        in >> pesi_pietre[i] >> energia_pietre[i];
        citta[i]=-1;
    }

	//citta in cui sono presenti le pietre
    for (int i=0; i<M; i++){
        in >> num_citta[i];
        location[i].resize(num_citta[i]);
        for (int k=0; k<num_citta[i]; k++) {
            in >> location[i][k];
        }
    }
     
    //matrice di adiacenza
    vector<vector<int> > mat;
    mat.resize(N);
    for (int i=0; i<N; i++) mat[i].resize(N);
    for (int i=0; i<N; i++) mat[i][i]=0;
    for (int i=1; i<N; i++) {
        for (int k=0; k<i; k++) {
            in >> mat[i][k];
            mat[k][i]=mat[i][k];
        }
    }

	//zaino
	if (C!=0) {
		 printknapSack(C,pesi_pietre,energia_pietre,M);
    
		//riordino le pietre scelte dallo zaino in ordine decrescente di peso e, in caso di parita, di energia
	    for (int i=0; i<zaino.size(); i++) {
	        for (int j=i; j<zaino.size(); j++) {
	            if (pesi_pietre[i]<pesi_pietre[j]) {
	                int appo=zaino[i];
	                zaino[i]=zaino[j];
	                zaino[j]=appo;
	            }
	            else if (pesi_pietre[i]==pesi_pietre[j]&&energia_pietre[i]<energia_pietre[j]){
	            	int appo=zaino[i];
	                zaino[i]=zaino[j];
	                zaino[j]=appo;
				}
	        }
	    }
	
	    //lista delle citta da visitare e in cui prendere le pietre messe al contrario
	    citta_contr.push_back(S);
	    bool prese [N];
	    int i,j;
	    for (i=0; i<N; i++) {
	        prese[i]=false;
	    }
	    vector<int> possibilita;
	    //parto da quella più pesante: se ha solo una citta possibile la prendo da li, altrimenti cerco l'arco meno pesante tra il nodo scelto in precedenza e questo
	    for (i=0; i<zaino.size(); i++) {
	        for (j=0; j<num_citta[zaino[i]]; j++) {
	        	if (!prese[location[zaino[i]][j]])	possibilita.push_back(location[zaino[i]][j]);
			}
	        if (possibilita.size()==1){
	            prese[possibilita[0]]=true;
	            citta[zaino[i]]=possibilita[0];
	            if (possibilita[0]!=S) citta_contr.push_back(possibilita[0]);
			}
			else {
				int min=mat[possibilita[0]][citta_contr[citta_contr.size()-1]];
				int citta_min=possibilita[0];
				for (j=1; j<possibilita.size(); j++) {
					if (mat[possibilita[j]][citta_contr[citta_contr.size()-1]]<min) {
						min = mat[possibilita[j]][citta_contr.size()-1];
						citta_min=possibilita[j];
					}
				}
				citta[zaino[i]]=citta_min;
				prese[citta_min]=true;
				if (citta_min!=S) citta_contr.push_back(citta_min);
			}
			possibilita.clear();
	    }
		
		/*cout << endl;
		for (i=0; i<citta_contr.size(); i++) {
			cout<<citta_contr[i]<<"  ";
		}
		cout<<endl;
		*/
	}
	
	//dall'ultimo nodo in cui ho preso la pietra più leggera cerco l'arco meno pesante che non vada a una citta già visitata e prendo quello
	int min = 100000;
	int citta_min = -1;
	if (C!=0) int nodo = citta_contr[citta_contr.size()-1];
	else {
		for (int i=0; i<N; i++) {
			if (i!=S) {
				if (mat[i][S]<min) {
					min = mat[i][S];
					citta_min = i;
				}
			}
		}
		nodo = citta_min;
	}
	while (nodo!=S)  {
		min = 100000;
		citta_min = -1;
		for (int i=0; i<N; i++) {
			if (i!=nodo && !prese[i] && i!=S) {
				if (mat[i][nodo]<min) {
					min=mat[i][nodo];
					citta_min=i;
				}
			}
		}
		if (citta_min!=-1) {
			prese[citta_min]=true;
			nodo=citta_min;
			citta_contr.push_back(citta_min);
			//cout<<citta_min<<endl;
		}
		bool test=true;
		for (int i=0; i<N; i++) {
			if (i!=S) test = test && prese[i];
		}
		if (test) nodo=S;
	}
	citta_contr.push_back(S);
    
    vector <int> path;
    //path_tot.resize(N+1);
    for (int i=citta_contr.size()-1; i>=0; i--){
    	path.push_back(citta_contr[i]);
	}
	
	cout<<endl;
	for (int i=0; i<N+1; i++) cout<<path[i]<<"  ";
	cout<<endl;
	
	
	int G=0; //somma energia pietre raccolte
	for (int i=0; i<M; i++) {
		if (citta[i]!=-1) G=G+energia_pietre[i];
	}
	
	int T=0; //tempo totale per percorrere il grafo 
	int W=0; //massa parziale delle pietre nel guanto
	for (int i=0; i<N+1; i++) {
		if (citta[]!=-1) W=W+massa_pietre[]
	}
	
	
   
    ofstream out ("output.txt");

}


void printknapSack(int W, int wt[], int val[], int n)
{
    int i, w;
    int K[n + 1][W + 1];
 
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] = max(val[i - 1] +
                    K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }
 
    // stores the result of Knapsack
    int res = K[n][W];   
    //printf("%d\n", res);
     
    w = W;
    for (i = n; i > 0 && res > 0; i--) {


        if (res == K[i - 1][w])
            continue;       
        else {
 
            // This item is included.
           // cout << i - 1 << endl;
             zaino.push_back(i-1);
            // Since this weight is included its
            // value is deducted
            res = res - val[i - 1];
            w = w - wt[i - 1];
        }
    }
}




double velocita(double vmax, double vmin, int W, int C) {
    return vmax - W*(vmax-vmin)/C;
}


double tempo_arco(int dis, double vel) {
    return dis/vel;
}
