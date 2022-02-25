#include "myheader.h"


//function 1
//This function collect data of dishes from the file Manot.txt, and add the dishes to a kitchen struct, and link them together.
void CreateProducts(FILE* out, FILE* in, P_kitchen* p)
{
	P_kitchen keeper; //keeper will save the name and place of a certain product.
	Kitchen* temp;
	char tmp[51]; //Maximum name size.
	int len, flag = 0;
	keeper = *p;
	temp = (Kitchen*)malloc(sizeof(Kitchen));
	if (!temp)
	{
		free(temp);
		Error_Msg(out,"Memmory Error");
	}
	while (fscanf(in, "%s %d %d", tmp, &temp->Quantity, &temp->Price) == 3) //if the scanned value is ok  -> fscanf return 3.
	{
		if (temp->Price <= 0) //check if the price is a valid number or not.
		{
			free(temp->ProductName);
			free(temp);
			fprintf(out,"\nThe price of %s must be a positive number.\n", tmp);
			flag = 1;

		}
		if (temp->Quantity <= 0) // check if the quantity is a valid number or not.
		{
			free(temp->ProductName);
			free(temp);
			fprintf(out, "\nThe quantity of %s must be a positive number.\n", tmp);
			flag = 1;
		}
		
		//comparing names
		if (*p != NULL)
		{
			while (keeper) //This loop will check if theres a product with the same name.
			{
				if (strcmp(keeper->ProductName, tmp) == 0) //check if the product name and the current keeper name is the same.
				{
					fprintf(out,"-- * It looks like the product %s already exists in the kitchen\n", tmp);
					free(temp->ProductName);
					free(temp);
					keeper = NULL;
					flag = 1;
				}
				else
				{
					keeper = keeper->next;       //keep searching if it wasn't found yet. 
				}
			}
		}
		len = strlen(tmp);
		temp->ProductName = (char*)malloc((len + 1) * sizeof(char)); //allocating the product name again.
		if (!temp)
		{
			free(temp);
			Error_Msg(out, "Memmory Allocation problem.\n exiting now.");
		}
		strcpy(temp->ProductName, tmp);
		temp->MaxQuantity = 0;

		if (flag == 0) // if flag = 0, it means that there is no problem, and we can continue and link the product to the previous products.
		{
			if (*p == NULL) //if that's the first time
			{
				*p = temp;
				(*p)->next = NULL;
			}
			else
			{
				temp->next = *p;
				*p = temp;
			}
				fprintf(out,"Successfully added a new product (%s) to the list (Quantity = %d, Price = %d)\n", temp->ProductName, temp->Quantity, temp->Price);
		}	
		//reset
		flag = 0;
		keeper = *p;
		//assigning memmory again.
		temp = (Kitchen*)malloc(sizeof(Kitchen));
		if (!temp)
		{
			free(temp);
			Error_Msg(out, "Memmory Error");
		}
	}
	keeper = NULL;
}

//function 2
//This function collect data of a dish to add new stock to, and add the new quantity to the kitchen struct.
void AddItems(FILE* out, char* prod, int q, P_kitchen* ptr)
{
	P_kitchen p;
	int flag = 0;
	p = *ptr;
	while (flag == 0)//if theres no problems about the collected values from the file.
	{
		if (p == NULL)//if theres no such product
		{
			flag = 1;
			fprintf(out,"Theres no such product as %s, please try again!\n", prod);
			return;
		}
		if (strcmp(p->ProductName, prod) == 0) //check if the product is appearing in the kitchen struct.
		{
			flag = 1;
			if (q <= 0) //check the stock.
			{
				fprintf(out,"%s don't have a valid quantity!\n", prod);
				return;
			}
			p->Quantity += q;  //adding the new quantity to the product.
		}
		else //if not, advancing to the next product in the struct.
		{
			p = p->next;
		}

	}

}			

//function 3
//This function collects a table number, and collect a order from it. the order will "consume" products from the kitchen, and add them as a linked list to Table struct.
void OrderItem(FILE* out, int TableNumber, char* ProductName, int Quantity, int MaxTablesNum, P_Table array, P_kitchen* p)
{
	P_kitchen TempKitch;
	P_Order TempOrder;
	TempKitch = *p;

	//checking if the number of table is valid
	if (TableNumber > MaxTablesNum || TableNumber < 1)
	{
		fprintf(out,"Error, No Table %d\n",TableNumber);
		fprintf(out,"Please choose a table between 1 to %d",MaxTablesNum);
		return;
	}
	//checking if the quantity of the ordered dish is valid
	if (Quantity <1)
	{
		fprintf(out,"The quantity most be a positive number");
		return;
	}
	
	while (TempKitch)
	{
		if (strcmp(TempKitch->ProductName, ProductName) == 0) //looking for the dish ordered in the product list.
		{
				if (TempKitch->Quantity < Quantity)//checking if the quantity is not valid
				{
					fprintf(out,"Error, not enough %s in stock\n", TempKitch->ProductName);
					return;
				}
				/*************************If that's the first order for this specific Table.***************************/
				if (array[TableNumber].orders == NULL)
				{
					//Memmory allocating a new node
					array[TableNumber].orders = (P_Order)malloc(sizeof(Order));
					if (array[TableNumber].orders == NULL)
					{
						free(array[TableNumber].orders);
						Error_Msg(out,"Memmory Allocation problem\n");
					}
					//Memmory Allocating the product name.
					array[TableNumber].orders->ProductName = (char*)malloc((strlen(ProductName) + 1) * (sizeof(char)));
					if (array[TableNumber].orders->ProductName == NULL)
					{
						free(array[TableNumber].orders->ProductName);
						free(array[TableNumber].orders);
						Error_Msg(out,"Memmory Allocation problem\n");	
					}
					strcpy(array[TableNumber].orders->ProductName, ProductName);
					
					//inserting the information to the current table.
					array[TableNumber].orders->Price = TempKitch->Price;
					array[TableNumber].orders->Quantity = Quantity;
					TempKitch->MaxQuantity += Quantity; //max quantity will be used later, in order to provide information about the most used dish of the day.
					array[TableNumber].orders->next = NULL;
					array[TableNumber].orders->prev = NULL;
					array[TableNumber].bill = Quantity * TempKitch->Price;	//updating the current table bill.
					TempKitch->Quantity -= Quantity; //decreasing the quantity.
					fprintf(out,"- New table opened (%d). first dish is %s, ordered %d times\n\n", TableNumber, ProductName, Quantity);
					return;
				}

				/*************************If that's NOT the first order for this specific Table.***************************/

				TempOrder = array[TableNumber].orders; //gets the head of the certain table.

				while (TempOrder)//TempOrder will get the head of the table array at the first iteration.
				{
					if (strcmp(array[TableNumber].orders->ProductName, ProductName) == 0) //checking for the product in order to "consume" it from the kitchen.
					{
						TempKitch->MaxQuantity += Quantity;
						array[TableNumber].orders->Quantity += Quantity;
						array[TableNumber].bill += Quantity * TempKitch->Price;
						TempKitch->Quantity -= Quantity;
						fprintf(out,"-- * The quantity of %s in the table %d was updated from %d to %d\n\n", ProductName, TableNumber,
							array[TableNumber].orders->Quantity, array[TableNumber].orders->Quantity + Quantity);
						return;
					}
					TempOrder = TempOrder->next; //moving to the next item of the struct.
				}

				//Create new node for the order
				TempOrder = (Order*)malloc(sizeof(Order));
				if (TempOrder == NULL)
				{
					free(array[TableNumber].orders);
					Error_Msg(out,"Memmory Allocation problem\n");
				}
				//Allocate memory for product name
				TempOrder->ProductName = (char*)malloc((strlen(ProductName) + 1) * sizeof(char));
				if (TempOrder->ProductName == NULL)
				{
					free(array[TableNumber].orders->ProductName);
					free(array[TableNumber].orders);
					Error_Msg(out,"Memmory Allocation problem\n");
				}
				//by reaching this point - it means that the order was taken and now we can advance the values inside.
				strcpy(TempOrder->ProductName, ProductName);
				TempOrder->Quantity = Quantity;
				TempOrder->Price = TempKitch->Price;
				TempOrder->prev = NULL;
				TempOrder->next = array[TableNumber].orders;
				array[TableNumber].orders->prev = TempOrder;
				array[TableNumber].bill += Quantity * TempKitch->Price; //sum up the bill.
				TempKitch->Quantity -= Quantity;
				fprintf(out,"- %d %s was added to the table %d \n\n", Quantity, ProductName, TableNumber + 1);
			
		}
		TempKitch = TempKitch->next; //progressing the kitchen
	}
	//if reached this point, it means don't have such dish in our menu, therefore - free the memory. 
	fprintf(out,"Can't add %s since it's not in the menu\n", ProductName);

}

//function 4
//This function collects a table number, a certain product and a quantity, and delete the quantity from the order.
void RemoveItem(FILE* out, int TableNumber, char* ProductName, int Quantity, int MaxTablesNum, P_Table array)
{
	P_Order TempOrder;

	if (TableNumber > MaxTablesNum || TableNumber < 1) //checking if the table exist.
	{
		fprintf(out,"Error, No Table %d\n", TableNumber);
		fprintf(out,"Please choose a table between 0 to %d", MaxTablesNum-1);
		return;
	}

	if (Quantity < 1)//checking if the quantity to remove is positive or not.
	{
		fprintf(out,"You cannot remove %d items from the order.\nPlease type a positive quantity", Quantity);
		return;
	}
	if (array[TableNumber].orders != NULL) //checking if the table is not empty.
	{
		while (array[TableNumber].orders != NULL) //while the table is not empty.
		{
			if (strcmp(ProductName, array[TableNumber].orders->ProductName) == 0) //check if the specific dish was ordered to this specific table
			{
				if (array[TableNumber].orders->Quantity < Quantity)//checks if the remove quantity is larger than the quantity ordered.
				{
					fprintf(out,"You cannot cancel %d %ss since you ordered %d dishes\n", Quantity, ProductName, array[TableNumber].orders->Quantity); 
					return;
				}
				else//if the removed quantity is ok -> reduce the quantity from the table, and reduce the bill.
				{
					array[TableNumber].bill -= Quantity * array[TableNumber].orders->Price;
					array[TableNumber].orders->Quantity -= Quantity;
					fprintf(out,"--Successfully removed %d %ss from the table number %d", Quantity, ProductName, TableNumber);
					return;
				}

				if (array[TableNumber].orders->Quantity == 0) //if theres no items left at the table.
				{
					TempOrder = array[TableNumber].orders;
					if (array[TableNumber].orders->prev == NULL) //if its the head
					{
						array[TableNumber].orders->next->prev = NULL;
						array[TableNumber].orders = array[TableNumber].orders->next;
						fprintf(out,"- Removing the %s from the table %d becuse the quantity equel to 0\n", ProductName, TableNumber);
						free(TempOrder);
					}
					else if (array[TableNumber].orders->next == NULL) //if its the tail
					{

						array[TableNumber].orders->prev->next = NULL;
						array[TableNumber].orders = array[TableNumber].orders->prev;
						fprintf(out,"- Removing the %s from the table %d becuse the quantity equel to 0\n", ProductName, TableNumber);
						free(TempOrder);
					}
					else //if its in the middle.
					{
						array[TableNumber].orders->prev->next = array[TableNumber].orders->next;
						array[TableNumber].orders->next->prev = array[TableNumber].orders->prev;
						array[TableNumber].orders = array[TableNumber].orders->prev;
						fprintf(out,"- Removing the %s from the table %d becuse the quantity equel to 0\n", ProductName, TableNumber);
						free(TempOrder);
					}
				}
				fprintf(out,"- successfully removed %d %ss from table number %d", Quantity, ProductName, TableNumber);
				return;
			}

			array[TableNumber].orders = array[TableNumber].orders->next; //advance to the next on the list.
		}

	}
	fprintf(out,"Cannot find a order to delete. please order first\n");
	return;
}

//function 5
//This function removes the collected table, prints the bill, and delete it from the table list.
void RemoveTable(FILE* out, int TableNumber, int MaxTablesNum, P_Table array, P_kitchen* k)
{
	int count = 0, i = 0, max = 0, len = 0;
	char favdish[50]; //to hold the favorite dish name.
	P_Order temp = NULL;

	if (TableNumber > MaxTablesNum || TableNumber < 1) //check if the table is within the limits.
	{
		fprintf(out,"Error, No Table %d\n", TableNumber);
		fprintf(out,"Please choose a table between 1 to %d", MaxTablesNum-1);
		return;
	}

	for (i = 0; i < MaxTablesNum; i++) //checking if that's the last table of the day
	{
		if (array[i].orders != NULL)
			count++;
	}

	if (array[TableNumber].bill == 0)//checking if the table hasn't ordered anything/ordered and canceled all.
	{
		fprintf(out,"\nTable number %d hasn't ordered yet.\n", TableNumber);
		return;
	}

	//*********************************Closing the table****************************************************

		fprintf(out,"\n\n		******************The bill********************\n");
		fprintf(out,"		****************Table No.%d*******************\n\n", TableNumber);
		fprintf(out,"                    Dish\t             Quantity\t            Total Price\n");
		while (array[TableNumber].orders != NULL) //setting the memory of the table free.
		{
			temp = array[TableNumber].orders;
			fprintf(out,"%25s\t\t%14d\t\t\t%10d NIS\n", temp->ProductName, temp->Quantity, temp->Quantity * temp->Price);
			array[TableNumber].orders = array[TableNumber].orders->next;
			free(temp->ProductName);
			free(temp);
		}

		fprintf(out,"\n\n**************************Total bill is:%d NIS********************************\n", array[TableNumber].bill);
		array[TableNumber].bill = 0;
		free(array[TableNumber].orders);
	
	//**************************************************************************************************************

		//will enter this if only if that's the last table of the day.
		//checking the favorite dish of the day.
		if (count == 1)
		{
			while ((*k) != NULL)
			{
				if ((*k)->MaxQuantity > max)
				{
					max = (*k)->MaxQuantity;
					strcpy(favdish, (*k)->ProductName);
				}
				(*k) = (*k)->next;
			}
			fprintf(out,"\n\nEnd of the day report:\n");
			fprintf(out,"The most ordered dish of the day was %s, with a total quantity of %d orders\n", favdish, max);
		}

		return;

}


void Error_Msg(FILE* out, char* msg)
{
	fprintf(out,"\n%s", msg);
	exit(1);
}

