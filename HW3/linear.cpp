    bool allTrue(const double a[], int n)
	{
		if (n <= 0) return true;
		if (n == 1) return somePredicate(a[0]);
		else return (somePredicate(a[n - 1]) & allTrue(a, n - 1));
	}

	int countFalse(const double a[], int n)
	{
		if (n <= 0) return 0;
		if (n == 1)
		{
			if (!somePredicate(a[0])) return 1;
			else return 0;
		}
		else
		{
			if (!somePredicate(a[n - 1])) return (1 + countFalse(a, n - 1));
			else return countFalse(a, n - 1);
		}
	}

	int firstFalse(const double a[], int n)
	{
		if (n <= 0) return -1;
		if (!somePredicate(a[0])) return 0;
		else
		{
			int d = firstFalse(a + 1, n - 1);
			if (d == -1) return -1;
			else return 1 + d;
		}
	}


	int indexOfMin(const double a[], int n)
	{
		if (n <= 0) return 0;
		if (n == 2)
		{
			if (a[1] < a[0]) return 1;
			else return 0;
		}
		else
		{
			int d = 1 + indexOfMin(a + 1, n - 1);
			if (a[0] <= a[d]) return 0;
			else return d;
		}
	}


	bool includes(const double a1[], int n1, const double a2[], int n2)
	{
		if (n2 <= 0) return true;
		if (n1 <= 0) return false;
		if (n2 == 1 && a1[0] == a2[0]) return true;
		else if (n1 == 1 && a1[0] != a2[0]) return false;
		else
		{
			if (a1[0] == a2[0]) return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
			else return includes(a1 + 1, n1 - 1, a2, n2);
		}
	}