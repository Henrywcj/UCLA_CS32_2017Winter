void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->name() != "") {
		if (path == "") cout << path + m->name() << endl;
		else cout << path + '/' + m->name() << endl;
	}
	if (m->menuItems() == NULL)
	{
		return;
	}
	for (int i = 0; i < (*(m->menuItems())).size(); i++)
		if (path != "") listAll((*(m->menuItems()))[i], path + '/' + m->name());
		else listAll((*(m->menuItems()))[i], path + m->name());
}
