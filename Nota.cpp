#include"Nota.h"

Nota::Nota(int nota1,int duracion1)
{
	nota = nota1;
	duracion = duracion1;
}

int Nota::getNota()
{
	return nota;
}

int Nota::getDuracion()
{
	return duracion;
}
