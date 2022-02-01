#include <bits/stdc++.h>
using namespace std;

struct TrieNode
{
    bool isWord;
    TrieNode* children[26];
};

TrieNode* getNewNode()
{
    TrieNode* temp = new TrieNode;
    temp->isWord = false;

    for (int i = 0; i < 26; i++)
    {
        temp->children[i] = NULL;
    }

    return temp;
}

void insert(TrieNode* root, string key)
{
    TrieNode* temp = root;

    for (int i = 0; i < (int)key.length(); i++)
    {
        int idx = int(key[i]) - 'a';
        if ( temp->children[idx] == NULL )
        {
            temp->children[idx] = getNewNode();
        }
        temp = temp->children[idx];
    }

    temp->isWord = true;
}

bool search(TrieNode* root, string key)
{
    if ( root == NULL) return  false;

    TrieNode* temp = root;

    for (int i = 0; i < (int)key.length(); i++)
    {
        int ind = int(key[i]) - 'a';
        if ( temp->children[ind] == NULL ) return false;
        temp = temp->children[ind];
    }

    return temp->isWord;
}

TrieNode* helperFunction(TrieNode* root, string key, int idx)
{
    // if tree is empty
    if (root == NULL) return NULL;

    //  helperFunction takes 3 parameters :-
    //  (1) root of subtrie (or subtree)
    //  (2) processing key (key which is going to be deleted)
    //  (3) idx (index) -> which character is processing or how much depth

    //  it returns root of this subtrie

    bool isEmpty = true;
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            isEmpty = false;
            break;
        }
    }

    if (idx == key.length())
    {
        /* hit the end of key
         possibilty -> 1) this key is prefix of other key,
         int this case,set isWord of current root equals to false;
         possibilty ->2) this key is not prefix of any other key
         in this case delete this node;
        */
        if (isEmpty == false)
        {
            root->isWord = false; // case 1
        }
        else
        {
            delete(root);
            root = NULL;   // case 2
        }

        return root;
    }

    int position = int(key[idx]) - 'a';
    root->children[position] = helperFunction(root->children[position], key, idx + 1);

    isEmpty = true;
    for (int i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            isEmpty = false;
            break;
        }
    }

    // Here we have two cases :-
    // (1) if all  children nodes of root is empty and this root is not marked as
    //     of word then delete this node

    // (2) if all children nodes are not empty "OR" this root os marked as end of
    //     word then do nothing

    if ( isEmpty && root->isWord == false )
    {
        delete(root);
        root = NULL;
    }

    return root;
}


TrieNode* deleteKey(TrieNode* root , string key)
{
    // if trie is empty
    if (root == NULL) return root;

    // if key is empty
    if (key == "") return root;

    // deleteKey function takes two parameters
    // (1) root of trie and
    // (2) key to be deleted

    // deleteKey function returns root of modified trie.

    root = helperFunction( root, key, 0 );

    return root;
}

void findAllWords( TrieNode* root, string key, vector<string> &all_words)
{
    if (root == NULL) return ;
    TrieNode* temp = root;

    for (int i = 0; i < (int)key.size(); i++)
    {
        int ind = int(key[i]) - 'a';
        // if children node at ind of current root is NULL this prefix do not exist in trie
        if ( temp->children[ind] == NULL ) return ;

        temp = temp->children[ind];
    }

    // if this prefix exist in trie then do BFS from current node to find all words
    queue<pair<TrieNode*, string> > q;

    q.push(make_pair(temp, key));
    string this_word;

    while (!q.empty())
    {
        temp = q.front().first;
        this_word = q.front().second;

        // if this is word ,then add it to the all_words(vector)
        if (temp->isWord) all_words.push_back( this_word );
        q.pop();

        for (int i = 0; i < 26; i++)
        {
            if ( temp->children[i] != NULL )
            {
                q.push( make_pair( temp->children[i] , this_word + char(i + int('a')) ) );
            }
        }
    }
}

void autoComplete(TrieNode* root, string key)
{
    // This function takes two parameters :-
    // (1) root node of trie and
    // (2) key for which suggestion need to find.

    // This function prints all the words for which key is prefix.

    // if key is empty , no need to print
    if (key.size() == 0) return ;

    // if key is not empty , then we will print all words in trie for which key is PREFIX.
    vector<string> all_words;

    findAllWords(root, key, all_words);

    // print all words
    if ( all_words.size() == 0  )
    {
        cout << "There are no suggestions." << '\n';
        return ;
    }

    cout << "There are " << all_words.size() << " suggestions for this prefix " << key << ":" << '\n';
    for (int i = 0; i < (int)all_words.size(); i++) {
        cout << all_words[i] << ' ';
    }

    return ;
}

int main()
{
    // creating a dictionary
    int n;
    cin >> n;

    vector<string> dict;

    string key;
    for (int i = 0; i < n; i++)
    {
        cin >> key;
        dict.push_back(key);
    }

    // creates root node
    TrieNode* root = getNewNode();

    // This for loop inserts all words of dictionary to trie
    for (int i = 0; i < dict.size(); i++)
    {
        insert(root, dict[i]);
    }

    cout << "Enter key to auto-complete :" << '\n';
    cin >> key ;

    autoComplete(root, key);
    return 0;
}
