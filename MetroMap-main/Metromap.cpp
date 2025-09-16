#include <bits/stdc++.h>

using namespace std;

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> nbrs;
    };

    static unordered_map<string, Vertex> vtces;

    Graph_M()
    {
        vtces.clear();
    }

    int numVetex()
    {
        return vtces.size();
    }

    bool containsVertex(string vname)
    {
        return vtces.count(vname) > 0;
    }

    void addVertex(string vname)
    {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(string vname)
    {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (auto it = vtx.nbrs.begin(); it != vtx.nbrs.end(); it++)
        {
            keys.push_back(it->first);
        }

        for (string key : keys)
        {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges()
    {
        int count = 0;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            Vertex vtx = it->second;
            count += vtx.nbrs.size();
        }

        return count / 2;
    }

    bool containsEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0)
        {
            return false;
        }

        return true;
    }

    void addEdge(string vname1, string vname2, int value)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) > 0)
        {
            return;
        }

        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    void removeEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0)
        {
            return;
        }

        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    void display_Map()
    {
        cout << "\t Kolkata Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << endl;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            string str = key + " =>" + "\n";
            Vertex vtx = it->second;
            for (auto it2 = vtx.nbrs.begin(); it2 != vtx.nbrs.end(); it2++)
            {
                string nbr = it2->first;
                str += "\t" + nbr + "\t";
                if (nbr.length() < 16)
                    str += "\t";
                if (nbr.length() < 8)
                    str += "\t";
                str += to_string(it2->second) + "\n";
            }
            cout << str << endl;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << endl;
    }

    void display_Stations()
    {
        cout << "\n***********************************************************************\n" << endl;
        int i = 1;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            cout << i << ". " << key << endl;
            i++;
        }
        cout << "\n***********************************************************************\n" << endl;
    }

    bool hasPath(string vname1, string vname2, unordered_map<string, bool> &processed)
    {
        if (containsEdge(vname1, vname2))
        {
            return true;
        }

        processed[vname1] = true;

        Vertex vtx = vtces[vname1];
        for (auto it = vtx.nbrs.begin(); it != vtx.nbrs.end(); it++)
        {
            string nbr = it->first;
            if (!processed.count(nbr))
            {
                if (hasPath(nbr, vname2, processed))
                {
                    return true;
                }
            }
        }

        return false;
    }

    class DijkstraPair
    {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair &other) const
        {
            return this->cost > other.cost;
        }
    };

    int dijkstra(string src, string des, bool nan)
    {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> pq;

        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            DijkstraPair np;
            np.vname = key;
            np.cost = INT_MAX;

            if (key == src)
            {
                np.cost = 0;
                np.psf = key;
            }

            pq.push(np);
            map[key] = np;
        }

        while (!pq.empty())
        {
            DijkstraPair rp = pq.top();
            pq.pop();

            if (rp.vname == des)
            {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            Vertex v = vtces[rp.vname];
            for (auto it = v.nbrs.begin(); it != v.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (map.count(nbr))
                {
                    int oc = map[nbr].cost;
                    Vertex k = vtces[rp.vname];
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * k.nbrs[nbr];
                    else
                        nc = rp.cost + k.nbrs[nbr];

                    if (nc < oc)
                    {
                        DijkstraPair gp = map[nbr];
                        gp.psf = rp.psf + nbr;
                        gp.cost = nc;

                        pq.push(gp);
                    }
                }
            }
        }
        return val;
    }

    class Pair
    {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_dis;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.nbrs.begin(); it != rpvtx.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
                    stack.push_front(np);
                }
            }
        }
        ans += to_string(min);
        return ans;
    }

    string Get_Minimum_Time(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_time;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.nbrs.begin(); it != rpvtx.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.nbrs[nbr];
                    stack.push_front(np);
                }
            }
        }
        double minutes = ceil((double)min / 60);
        ans += to_string(minutes);
        return ans;
    }

    vector<string> get_Interchanges(string str)
    {
        vector<string> arr;
        stringstream ss(str);
        string token;
        while (getline(ss, token, ' '))
        {
            arr.push_back(token);
        }
        return arr;
    }

    static void Create_Metro_Map(Graph_M &g)
    {
        g.addVertex("Howrah Maidan");
        g.addVertex("Chandini Chowk");
        g.addVertex("Esplanade");
        g.addVertex("Victoria");
        g.addVertex("MG Road");
        g.addVertex("Shyambazar");
        g.addVertex("Dum Dum");
        g.addVertex("Dakshineswar");
        g.addVertex("Rabindra Sadan");
        g.addVertex("Netaji Bhavan");
        g.addVertex("Kalighat");
        g.addVertex("Mahanayak Uttam Kumar");
        g.addVertex("Masterda Surya Sen");
        g.addVertex("Satyajti Ray");
        g.addVertex("Sector V");
        g.addVertex("Technopolis");
        g.addVertex("Bidhan Nagar");
        g.addVertex("New Town");
        g.addVertex("Dum Dum Park");
        g.addVertex("Biman Bandar");
        g.addVertex("City Center");

        g.addEdge("Howrah Maidan", "Chandini Chowk", 8);
        g.addEdge("Chandini Chowk", "Esplanade", 10);
        g.addEdge("Esplanade", "Victoria", 8);
        g.addEdge("Esplanade", "MG Road", 6);
        g.addEdge("MG Road", "Shyambazar", 9);
        g.addEdge("Shyambazar", "Dum Dum", 7);
        g.addEdge("Dum Dum", "Dakshineswar", 6);
        g.addEdge("Rabindra Sadan", "Netaji Bhavan", 15);
        g.addEdge("Netaji Bhavan", "Kalighat", 6);
        g.addEdge("Kalighat", "Mahanayak Uttam Kumar", 7);
        g.addEdge("Mahanayak Uttam Kumar", "Masterda Surya Sen", 1);
        g.addEdge("Masterda Surya Sen", "Satyajti Ray", 2);
        g.addEdge("Satyajti Ray", "Sector V", 5);
        g.addEdge("Sector V", "Technopolis", 2);
        g.addEdge("Technopolis", "Bidhan Nagar", 7);
        g.addEdge("Bidhan Nagar", "New Town", 8);
        g.addEdge("Sector V", "Dum Dum Park", 2);
        g.addEdge("Dum Dum Park", "Biman Bandar", 2);
        g.addEdge("Sector V", "City Center", 3);
    }


    static vector<string> printCodelist()
    {
        cout << "List of station along with their codes:\n" << endl;
        int i = 1, j = 0, m = 1;
        string temp = "";
        vector<string> codes(vtces.size());
        char c;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            stringstream ss(key);
            codes[i - 1] = "";
            j = 0;
            while (getline(ss, temp, ' '))
            {
                c = temp[0];
                while (c > 47 && c < 58)
                {
                    codes[i - 1] += c;
                    j++;
                    c = temp[j];
                }
                if ((c < 48 || c > 57) && c < 123)
                    codes[i - 1] += c;
            }
            if (codes[i - 1].length() < 2)
                codes[i - 1] += toupper(temp[1]);

            cout << i << ". " << key << "\t";
            if (key.length() < (22 - m))
                cout << "\t";
            if (key.length() < (14 - m))
                cout << "\t";
            if (key.length() < (6 - m))
                cout << "\t";
            cout << codes[i - 1] << endl;
            i++;
            if (i == pow(10, m))
                m++;
        }
        return codes;
    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

class Heap
{
public:
    vector<Graph_M::DijkstraPair> data;
    unordered_map<string, int> map;

    void add(Graph_M::DijkstraPair item)
    {
        data.push_back(item);
        map[item.vname] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void upheapify(int ci)
    {
        int pi = (ci - 1) / 2;
        if (isLarger(data[ci], data[pi]) > 0)
        {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void swap(int i, int j)
    {
        Graph_M::DijkstraPair ith = data[i];
        Graph_M::DijkstraPair jth = data[j];

        data[i] = jth;
        data[j] = ith;
        map[ith.vname] = j;
        map[jth.vname] = i;
    }

    void display()
    {
        for (int i = 0; i < data.size(); i++)
        {
            cout << data[i].vname << " ";
        }
        cout << endl;
    }

    int size()
    {
        return data.size();
    }

    bool isEmpty()
    {
        return size() == 0;
    }

    Graph_M::DijkstraPair remove()
    {
        swap(0, data.size() - 1);
        Graph_M::DijkstraPair rv = data[data.size() - 1];
        data.pop_back();
        downheapify(0);

        map.erase(rv.vname);
        return rv;
    }

    void downheapify(int pi)
    {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int mini = pi;

        if (lci < data.size() && isLarger(data[lci], data[mini]) > 0)
        {
            mini = lci;
        }

        if (rci < data.size() && isLarger(data[rci], data[mini]) > 0)
        {
            mini = rci;
        }

        if (mini != pi)
        {
            swap(mini, pi);
            downheapify(mini);
        }
    }

    Graph_M::DijkstraPair get()
    {
        return data[0];
    }

    int isLarger(Graph_M::DijkstraPair t, Graph_M::DijkstraPair o)
    {
        return t.cost - o.cost;
    }

    void updatePriority(Graph_M::DijkstraPair pair)
    {
        int index = map[pair.vname];
        upheapify(index);
    }
};

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\t\t\t****WELCOME TO THE Kolkata METRO APP*****" << endl;

    while (true)
    {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n" << endl;
        cout << "1. LIST ALL THE STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "7. EXIT THE MENU" << endl;
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";
        int choice = -1;
        cin >> choice;
        cout << "\n***********************************************************\n" << endl;
        if (choice == 7)
        {
            break;
        }
        switch (choice)
        {
        case 1:
            g.display_Stations();
            break;

        case 2:
            g.display_Map();
            break;

        case 3:
        {
            vector<string> stationNames;
            for (auto it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); ++it) {
                stationNames.push_back(it->first);
            }

            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER NAME OF STATIONS\n" << endl;
            cout << "ENTER YOUR CHOICE: ";
            int ch;
            cin >> ch;

            string st1 = "", st2 = "";
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1)
            {
                int s1, s2;
                cin >> s1 >> s2;
                if (s1 < 1 || s1 > stationNames.size() || s2 < 1 || s2 > stationNames.size()) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                    break;
                }
                st1 = stationNames[s1 - 1];
                st2 = stationNames[s2 - 1];
            }
            else if (ch == 2)
            {
                cin.ignore();
                getline(cin, st1);
                getline(cin, st2);
            }
            else
            {
                cout << "Invalid choice" << endl;
                break;
            }

            unordered_map<string, bool> processed;
            if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
                cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM" << endl;
            break;
        }

        case 4:
        {
            vector<string> stationNames;
            for (auto it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); ++it) {
                stationNames.push_back(it->first);
            }

            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER NAME OF STATIONS\n" << endl;
            cout << "ENTER YOUR CHOICE: ";
            int ch;
            cin >> ch;

            string st1 = "", st2 = "";
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1)
            {
                int s1, s2;
                cin >> s1 >> s2;
                if (s1 < 1 || s1 > stationNames.size() || s2 < 1 || s2 > stationNames.size()) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                    break;
                }
                st1 = stationNames[s1 - 1];
                st2 = stationNames[s2 - 1];
            }
            else if (ch == 2)
            {
                cin.ignore();
                getline(cin, st1);
                getline(cin, st2);
            }
            else
            {
                cout << "Invalid choice" << endl;
                break;
            }

            unordered_map<string, bool> processed;
            if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
                cout << "SHORTEST TIME FROM (" << st1 << ") TO (" << st2 << ") IS " << g.dijkstra(st1, st2, true) / 60 << " MINUTES" << endl;
            break;
        }

        case 5:
        {
            vector<string> stationNames;
            for (auto it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); ++it) {
                stationNames.push_back(it->first);
            }

            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER NAME OF STATIONS\n" << endl;
            cout << "ENTER YOUR CHOICE: ";
            int ch;
            cin >> ch;

            string s1 = "", s2 = "";
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1)
            {
                int idx1, idx2;
                cin >> idx1 >> idx2;
                if (idx1 < 1 || idx1 > stationNames.size() || idx2 < 1 || idx2 > stationNames.size()) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                    break;
                }
                s1 = stationNames[idx1 - 1];
                s2 = stationNames[idx2 - 1];
            }
            else if (ch == 2)
            {
                cin.ignore();
                getline(cin, s1);
                getline(cin, s2);
            }
            else
            {
                cout << "Invalid choice" << endl;
                break;
            }

            unordered_map<string, bool> processed2;
            if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                int len = str.size();
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "DESTINATION STATION : " << s2 << endl;
                cout << "DISTANCE : " << str[len - 1] << endl;
                cout << "~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << endl;
                for (int i = 1; i < len - 1; i++)
                {
                    cout << str[i] << endl;
                }
                cout << str[len - 2] << "   ==>    END";
            }
            break;
        }

        case 6:
        {
            vector<string> stationNames;
            for (auto it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); ++it) {
                stationNames.push_back(it->first);
            }

            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER NAME OF STATIONS\n" << endl;
            cout << "ENTER YOUR CHOICE: ";
            int ch;
            cin >> ch;

            string s1 = "", s2 = "";
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1)
            {
                int idx1, idx2;
                cin >> idx1 >> idx2;
                if (idx1 < 1 || idx1 > stationNames.size() || idx2 < 1 || idx2 > stationNames.size()) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                    break;
                }
                s1 = stationNames[idx1 - 1];
                s2 = stationNames[idx2 - 1];
            }
            else if (ch == 2)
            {
                cin.ignore();
                getline(cin, s1);
                getline(cin, s2);
            }
            else
            {
                cout << "Invalid choice" << endl;
                break;
            }

            unordered_map<string, bool> processed2;
            if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(s1, s2));
                int len = str.size();
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "DESTINATION STATION : " << s2 << endl;
                cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                cout << "~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << endl;
                for (int i = 1; i < len - 1; i++)
                {
                    cout << str[i] << endl;
                }
                cout << str[len - 2] << "   ==>    END";
            }
            break;
        }

        default:
            cout << "Invalid choice" << endl;
            break;
        }
        cout << "\n***********************************************************\n" << endl;
    }

    return 0;
}



