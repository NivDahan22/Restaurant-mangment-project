#include "myheader.h"

void main()
{
	int TablesQuant, i, check, option;
	int Quant, price, TableNumber;
	char product[50];
	FILE* Manot, * Inst, * output;
	P_kitchen p = NULL;
	P_Table TableArr;
	output = fopen("output.txt", "w");
	if (!output)
	{
		printf("Error while opening the output.txt file");
		exit(1);
	}
	Inst = fopen("Instructions.txt", "r");
	if (!Inst) Error_Msg(output,"Memmory Allocation Problem\nExiting now");
	
	fscanf(Inst, "%d", &TablesQuant); //collectinig the number of tables at the resturant.
	TableArr = (P_Table)malloc(TablesQuant * sizeof(Table));
	if(!TableArr)
	{
		free(TableArr);
		Error_Msg(output,"Memmory Allocation Problems");
	}
	for (i = 0; i < TablesQuant; i++)//reseting the table numbers, and the bills.
	{
		TableArr[i].bill = 0;
		TableArr[i].TableNumber = i;
		TableArr[i].orders = NULL;
	}
	while (fscanf(Inst, "%d", &option) ==1) //while the first scanned value of every line is ok.
	{
		switch (option)
		{
			case 1: //collecting all products in the resturant.
			{
				Manot = fopen("Manot.txt", "r");
				if (!Manot) Error_Msg(output,"Memmory Allocation Problem\nExiting now");
				CreateProducts(output,Manot, &p);
				fclose(Manot);
				break;
			}

			case 2: //Add more to an existing item stock.
			{
				fscanf(Inst, "%s %d", product, &Quant);
				AddItems(output,product, Quant, &p);
				break;
			}
			case 3:// Add an order to a table.
			{
				fscanf(Inst, "%d %s %d", &TableNumber, product, &Quant);
				OrderItem(output,TableNumber, product, Quant, TablesQuant, TableArr, &p);
				break;
			}

			case 4://Cancelation of a certain dish.
			{
				fscanf(Inst, "%d %s %d", &TableNumber, product, &Quant);
				RemoveItem(output,TableNumber, product, Quant, TablesQuant, TableArr);
				break;
			}
			case 5:
			{
				fscanf(Inst, "%d", &TableNumber);
				RemoveTable(output,TableNumber, TablesQuant, TableArr, &p);
				break;
			}
			default:
			{
				fprintf(output,"\n Theres no such command\n");
				break;
			}
		}
	}
	fclose(output);
	fclose(Inst);
	free(TableArr);
	return;
}