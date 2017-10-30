#include "city.hpp"


city::city(){
	srand(time(NULL));
	int x, y;
	std::cout<<"Filas: ";
	std::cin>>x;
	x_=x;
	std::cout<<"Columnas: ";
	std::cin>>y;
	y_=y;




	if(x_>ALTO | y_>ANCHO)metropolis=true;
	else metropolis=false;


	if(metropolis){
		x_mo=y_mo=0;
		x_mm=x_;
		y_mm=y_;
		if(x_>ALTO)x_mm=ALTO;
		if(y_>ANCHO)y_mm=ANCHO;
	}


	do{
		std::cout<<"Obstaculos: ";
		std::cin>>obs_;
	}while(obs_>((x*y)-2));

	c_=new int*[x+2];
	for(int i=0;i<x+2;i++)
		c_[i]=new int[y+2];


	for(int i=0;i<(x+2);i++){
		for(int j=0;j<(y+2);j++){
			if(i==0 || i==(x_+1) || j==0 || j==(y_+1))	c_[i][j]=0;
			else c_[i][j]=1;
		}
	}

	{
		bool respuesta_correcta=false;
		do{
			std::cout<<"Manual: (s/n)";
			char manual_respuesta;
			std::cin>>manual_respuesta;
			respuesta_correcta=true;
			if(manual_respuesta=='s')manual=true;
			else if(manual_respuesta=='n')manual=false;
			else {std::cout<<"Respuesta Incorrecta\n"; respuesta_correcta=false;}
		}while (!respuesta_correcta);
	}

	if(!manual){
		gen_car();
		gen_obs();
		gen_end();
	}
	else{
		excavadora();
	}


	//move();
	auto_move();
}


void city::excavadora(){
	c_[1][1]=5;
	int e_x=1;
	int e_y=1;
	int e_tx=1;
	int e_ty=1;

	int obs_exc=0;
	int ant_x=0;

	bool nosalir=true;

	char mov='e';

	int anterior=1;

	bool nocoche=true;
	bool novictory=true;

	do{
		if(anterior==2)ant_x=1;else ant_x=0;
		obs_=obs_exc+ant_x;
		imprimir();

		initscr(); 
		cbreak();
    	noecho();
    	nodelay(stdscr, TRUE);
    	scrollok(stdscr, TRUE);

    	usleep(90000);

		mov=getch();

		endwin();
	
		switch(mov){
			case 'w': if(e_x>1)e_tx--;
				break;
			case 'a': if(e_y>1)e_ty--;
				break;
			case 's': if(e_x<x_)e_tx++;
				break;
			case 'd': if(e_y<y_)e_ty++;
				break;
				
			case 'x': 
				if(anterior==3)nocoche=true; 
				if(anterior==4)novictory=true;
				anterior=2;break;
			case 'v': 
				if(novictory){
				if(anterior==3)nocoche=true; 
				anterior=4; novictory=false;}break;
			case 'c': 
				if(nocoche)  {nocoche=false;
					if(anterior==4)novictory=true;
					anterior=3;};break;
			case 'b': 
				if(anterior==3)nocoche=true; 
				if(anterior==4)novictory=true;
				anterior=1;break;
			case 'q': 
				c_[e_x][e_y]=anterior;
				nosalir=false; break;
		}

		if(e_tx!=e_x|e_ty!=e_y){

			if(anterior==3){
				x_car=e_x;
				y_car=e_y;
			}
			else if(anterior==4){
				x_v=e_x;
				y_v=e_y;
			}

			c_[e_x][e_y]=anterior;
			if(anterior==2)obs_exc++;

			if(c_[e_tx][e_ty]==2)obs_exc--;
			anterior=c_[e_tx][e_ty];
			c_[e_tx][e_ty]=5;
			e_x=e_tx;
			e_y=e_ty;
		}





	}while(nosalir);

	/*if(novictory! && nocoche!){ 
		std::cout<<"¿Guardar mapa? (s/n)";
		char a= getchar();
		if(a=='s'){
			savemap();
		}
	}*/
}

/*void city::savemap(){
	/*Ver si existe el archivo
			-SI-> mover a otra carpeta
	  Crear archivo nuevo
	  Volcar datos
	*/
	/*char place[] ="read_map/open.txt";
	std::ifstream fe; 
	fe.open(place);
	if(fe.is_open()==1){

	}
	else{

	}

}*/

int city::get_val(int x, int y){
	return c_[x][y];
}
void city::imprimir(){
	system("clear");
	std::cout<<"Filas: "<<x_<<"\tColumnas: "<<y_<<"\tObstaculos: "<<obs_<<"\n";
	for(int i=0;i<x_+2;i++){
		for(int j=0;j<y_+2;j++){
			if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
				else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
		}
		std::cout<<"\n";
	}
}
void city::imprimir_metropolis(){
	system("clear");
	std::cout<<"Filas: "<<x_<<"\tColumnas: "<<y_<<"\tObstaculos: "<<obs_<<"\n";
	for(int i=x_mo;i<x_mm+2;i++){
		for(int j=y_mo;j<y_mm+2;j++){
			if(get_val(i,j)==3)std::cout<<NEG<<col[3]<<'8'<<RST<<" ";

			else if(get_val(i,j)==4)std::cout<<col[4]<<'@'<<RST<<" ";
				else std::cout<<col[get_val(i,j)]<<'#'<<RST<<" ";
		}
		std::cout<<"\n";
	}
}

void city::gen_car(){
	x_car=((rand()%x_)+1);
	y_car=((rand()%y_)+1);
	c_[x_car][y_car]=3;
}
void city::gen_obs(){
	int contador=0;
	int x,y;
	while(contador<obs_){
		x=((rand()%x_)+1);
		y=((rand()%y_)+1);
		if(get_val(x,y)==1){
			c_[x][y]=2;
			contador++;
		}
	}
}
void city::gen_end(){
	int x,y;
	do{
		x=((rand()%x_)+1);
		y=((rand()%y_)+1);
	}while(get_val(x,y)!=1);
	c_[x][y]=4;
	x_v=x;
	y_v=y;
}

void city::move(){

	bool nollego=true;
	bool nosalir=true;

	mov='e';
	int x_tem,y_tem;
	do{
		if(metropolis)imprimir_metropolis();
		else imprimir();

		x_tem=x_car;
		y_tem=y_car;

		initscr(); 
		cbreak();
    	noecho();
    	nodelay(stdscr, TRUE);
    	scrollok(stdscr, TRUE);

    	usleep(90000);

		mov=getch();

		endwin();

		switch(mov){
			case 'w':x_tem-=1;break;
			case 'a':y_tem-=1;break;
			case 's':x_tem+=1;break;
			case 'd':y_tem+=1;break;

			case 'i': 
				if(x_mo>0){
					x_mo--;
					x_mm--;
				}
				break;
			
			case 'j': 
				if(y_mo>0){
					y_mo--;
					y_mm--;
				}
				break;
			case 'k': 
				if(x_mm<x_){
					x_mo++;
					x_mm++;
				}
				break;
			case 'l': 
				if(y_mm<y_){
					y_mo++;
					y_mm++;
				}
				break;

			case 'q': nosalir=false;
		}
		mov='e';

		if(c_[x_tem][y_tem]==1){
			c_[x_car][y_car]=1;
			c_[x_tem][y_tem]=3;
			x_car=x_tem;
			y_car=y_tem;
		}
		else if(c_[x_tem][y_tem]==4){
			nollego=false;
		}

		
	}while(nollego & nosalir);

	if(!nollego)std::cout<<col[4]<<"GANASTE\n\tGANASTE\n\t\tGANASTE\n\t\t\tGANASTE\n\t\t\t\tGANASTE\n"<<RST;
}


void city::auto_move(){
	mapa.resize(x_+2);
	for(int i = 0; i < x_+2; i++) {
		mapa[i].resize(y_+2);
		for(int j = 0; j < y_+2; j++)
			mapa[i][j] = false;
	}
	mapa[x_car][y_car] = true;

	bool nollego=true;
	bool nosalir=true;

	mov='e';
	int x_tem,y_tem;
	do {
		if(metropolis)imprimir_metropolis();
		else imprimir();

		x_tem=x_car;
		y_tem=y_car;


		usleep(90000);
		//std::cin >> mov;

		mov=get_next_move();


		switch(mov) {
			case 'w':x_tem-=1;break;
			case 'a':y_tem-=1;break;
			case 's':x_tem+=1;break;
			case 'd':y_tem+=1;break;

			case 'i':
				if(x_mo>0){
					x_mo--;
					x_mm--;
				}
				break;

			case 'j':
				if(y_mo>0){
					y_mo--;
					y_mm--;
				}
				break;
			case 'k':
				if(x_mm<x_){
					x_mo++;
					x_mm++;
				}
				break;
			case 'l':
				if(y_mm<y_){
					y_mo++;
					y_mm++;
				}
				break;

			case 'n': nosalir=false;
		}
		mov='e';

		if(c_[x_tem][y_tem]==1){
			c_[x_car][y_car]=1;
			c_[x_tem][y_tem]=3;
			x_car=x_tem;
			y_car=y_tem;
		}
		else if(c_[x_tem][y_tem]==4){
			nollego=false;
		}
		
		bool found = false;
		int i = 0;
		while((i < posibilidades.size()) && (!found)) {
			if((posibilidades[i][0] == x_car) && (posibilidades[i][1] == y_car))
				found = true;
			i++;
		}
		if(found) posibilidades.erase(posibilidades.begin() + i - 1);


	} while(nollego & nosalir);
	

	if(!nollego)std::cout<<col[4]<<"GANASTE\n\tGANASTE\n\t\tGANASTE\n\t\t\tGANASTE\n\t\t\t\tGANASTE\n"<<RST;
	else std::cout<<"No se puede llegar al destino\n";
}


char city::get_next_move() {
	char move;
	std::vector<int> aux, victoria = {x_v, y_v}, car = {x_car, y_car};
	std::vector<std::vector<int> > opciones, opciones_aux;

	if((get_val(x_car-1, y_car) == 1) || (get_val(x_car-1, y_car) == 4)) {
		aux = {x_car-1, y_car};
		opciones.push_back(aux);
	}
	if((get_val(x_car+1, y_car) == 1) || (get_val(x_car+1, y_car) == 4)) {
		aux = {x_car+1, y_car};
		opciones.push_back(aux);
    }
	if((get_val(x_car, y_car-1) == 1) || (get_val(x_car, y_car-1) == 4)) {
		aux = {x_car, y_car-1};
		opciones.push_back(aux);
    }
	if((get_val(x_car, y_car+1) == 1) || (get_val(x_car, y_car+1) == 4)) {
		aux = {x_car, y_car+1};
		opciones.push_back(aux);
    }

	std::vector<bool> borrar;
	borrar.resize(opciones.size());
	for(int i = 0; i < opciones.size(); i++) {
		if(mapa[opciones[i][0]][opciones[i][1]])
			borrar[i] = true;
	}
	opciones_aux = opciones;
	opciones.clear();
	for(int i = 0; i < opciones_aux.size(); i++) {
		if(!borrar[i])
			opciones.push_back(opciones_aux[i]);
	}
	

	for(int i = 0; i < opciones.size(); i++) {
		posibilidades.push_back(opciones[i]);
		mapa[opciones[i][0]][opciones[i][1]] = true;
	}


	if(!posibilidades.empty()) {
		int min = 9999999;
		int elegida = 0;
		for(int i = 0; i < posibilidades.size(); i++)
			if(f(posibilidades[i], victoria) + f(car, posibilidades[i])/2 < min) {
				min = f(posibilidades[i], victoria) + f(car, posibilidades[i])/2;
				elegida = i;
			}
		aux = encontrar_camino(posibilidades[elegida]);
		
		if(aux[0] == x_car) {
			if(aux[1] == y_car-1)
				move = 'a';
			else if(aux[1] == y_car+1)
				move = 'd';
		}
		else
			if(aux[0] == x_car-1)
				move = 'w';
			else if(aux[0] == x_car+1)
				move = 's';
	}
	else
		move = 'n';

	
	
	return move;
}


int city::f(std::vector<int> casilla, std::vector<int> objetivo) {
	return (abs(casilla[0] - objetivo[0]) + abs(casilla[1] - objetivo[1]));
}


std::vector<int> city::encontrar_camino(std::vector<int> objetivo) {
	std::set<recorrido> lista;
	recorrido aux;
	aux.add(x_car, y_car);
	lista.insert(aux);
	//std::cout << x_car << ',' << y_car << "   " << x_v << ',' << y_v << "   " << objetivo[0] << ',' << objetivo[1] << '\n';
	
	while((lista.begin()->get_end() != objetivo) && (!lista.empty())) {
		aux = *lista.begin();
		lista.erase(lista.begin());
		if(mapa[aux.get_end()[0] - 1][aux.get_end()[1]] && !aux.existe(aux.get_end()[0] - 1, aux.get_end()[1]))
			lista.insert(aux.create(aux.get_end()[0] - 1, aux.get_end()[1]));
		if(mapa[aux.get_end()[0] + 1][aux.get_end()[1]] && !aux.existe(aux.get_end()[0] + 1, aux.get_end()[1]))
			lista.insert(aux.create(aux.get_end()[0] + 1, aux.get_end()[1]));
		if(mapa[aux.get_end()[0]][aux.get_end()[1] - 1] && !aux.existe(aux.get_end()[0], aux.get_end()[1] - 1))
			lista.insert(aux.create(aux.get_end()[0], aux.get_end()[1] - 1));
		if(mapa[aux.get_end()[0]][aux.get_end()[1] + 1] && !aux.existe(aux.get_end()[0], aux.get_end()[1] + 1))
			lista.insert(aux.create(aux.get_end()[0], aux.get_end()[1] + 1));
	}
	
	if(lista.empty()) std::cout << "ALGO VA MAL, LAS POSIBILIDADES DEBEN ESTAR EN EL MAPA" << std::endl;
	
	return lista.begin()->get_first();
}




recorrido::recorrido() {
	Coste = -1;
}


std::vector<int> recorrido::get_first() const {
	return Camino[1];
}


std::vector<int> recorrido::get_end() const {
	return Camino.back();
}


void recorrido::add(int x, int y) {
	std::vector<int> aux = {x, y};
	Camino.push_back(aux);
	Coste++;
}


recorrido recorrido::create(int x, int y) {
	recorrido rec;
	rec.Camino = Camino;
	std::vector<int> aux = {x, y};
	rec.Camino.push_back(aux);
	rec.Coste = Coste + 1;

	return rec;
}


bool recorrido::existe(int x, int y) {
	bool existe = false;
	for(int i = 0; i < Camino.size(); i++)
		if((Camino[i][0] == x) && (Camino[i][1] == y))
			existe = true;

	return existe;
}


bool recorrido::operator<(const recorrido& rec) const {
	if(Coste != rec.Coste)
       return (Coste < rec.Coste);
	// si el coste es igual no importa el orden, pero hace falta criterio para que el set no los vea como iguales
	else {
		for(int i = 0; i < Camino.size(); i++) {
			if(Camino[i][0] < rec.Camino[i][0])
				return true;
			else if(Camino[i][0] > rec.Camino[i][0])
				return false;
			if(Camino[i][1] < rec.Camino[i][1])
				return true;
			else if(Camino[i][1] > rec.Camino[i][1])
				return false;
		}
	}
}


bool recorrido::operator>(const recorrido& rec) const {
	if(Coste != rec.Coste)
       return (Coste > rec.Coste);
	// si el coste es igual no importa el orden, pero hace falta criterio para que el set no los vea como iguales
	else {
		for(int i = 0; i < Camino.size(); i++) {
			if(Camino[i][0] > rec.Camino[i][0])
				return true;
			else if(Camino[i][0] < rec.Camino[i][0])
				return false;
			if(Camino[i][1] > rec.Camino[i][1])
				return true;
			else if(Camino[i][1] < rec.Camino[i][1])
				return false;
		}
	}
}


bool recorrido::operator==(const recorrido& rec) const {
	bool iguales = true;
	for(int i = 0; i < Camino.size(); i++)
		if((Camino[i][0] != rec.Camino[i][0]) || (Camino[i][1] != rec.Camino[i][1]))
			iguales = false;

	return iguales;
}






