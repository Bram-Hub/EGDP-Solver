#include "eg.h"

EG::~EG()
{
  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    delete (*iter);
  }
}

EG::EG(const EG & g)
{
  symbols = std::list<char>();
  cuts = std::list<EG*>();

  for(std::list<char>::const_iterator iter = g.symbols.begin(); iter != g.symbols.end(); ++iter)
  {
    symbols.push_back(*iter);
  }

  for(std::list<EG*>::const_iterator iter = g.cuts.begin(); iter != g.cuts.end(); ++iter)
  {
    EG* tmp = new EG(*(*iter));
    cuts.push_back(tmp);
  }
}

void EG::read(std::string & g, int & i)
{
  while(i < g.length())
  {
    switch(g[i])
    {
      case '\t':
      case '\n':
      case ' ':
        ++i;
        continue;
      case '(':
      {
        ++i;
        EG* temp = new EG();
        cuts.push_back(temp);
        temp->read(g,i);
      }
        break;
      case ')':
        return;
      default:
        symbols.push_back(g[i]);
        break;
    }
    ++i;
  }
}

std::string EG::toString()
{
  std::string s = "";
  for(std::list<char>::iterator iter = symbols.begin(); iter != symbols.end(); ++iter)
  {
    s = s+(*iter);
  }
  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    s = s+"("+(*iter)->toString()+")";
  }
  return s;
}

void EG::print(int depth)
{
  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    for(int j = 0; j<depth; ++j)
    {
      std::cout << "\t";
    }
    std::cout << "(" << (*iter)->toString() << ")\n";
  }
  for(std::list<char>::iterator iter = symbols.begin(); iter != symbols.end(); ++iter)
  {
    for(int j = 0; j<depth; ++j)
    {
      std::cout << "\t";
    }
    std::cout << *iter << "\n";
  }
  if(finished())
  {
    for(int j = 0; j<depth; ++j)
    {
      std::cout << "\t";
    }
    std::cout << "Open and Finished" << "\n";
  }
  std::cout << "\n";
}

void EG::reduce(char c, bool t)
{
  if(t)
  {
    for(std::list<char>::iterator iter = symbols.begin(); iter != symbols.end();)
    {
      if(*iter == c)
      {
        iter = symbols.erase(iter);
      }
      else
      {
        ++iter;
      }
    }
  }
  else
  {
    for(std::list<char>::iterator iter = symbols.begin(); iter != symbols.end();)
    {
      if(*iter == c)
      {
        iter = symbols.erase(iter);
        cuts.push_back(new EG());
      }
      else
      {
        ++iter;
      }
    }
  }

  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    (*iter)->reduce(c,t);
  }
}

bool EG::finished()
{
  return symbols.size() == 0 && cuts.size() == 0;
}

bool EG::open()
{
  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    if((*iter)->finished())
    {
      return false;
    }
  }
  return true;
}

void EG::simplify()
{
  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end(); ++iter)
  {
    (*iter)->simplify();
  }

  for(std::list<EG*>::iterator iter = cuts.begin(); iter != cuts.end();)
  {
    if((*iter)->cuts.size() == 1 && (*iter)->symbols.size() == 0)
    {
      for(std::list<char>::iterator iter2 = (*iter)->cuts.front()->symbols.begin();
          iter2 != (*iter)->cuts.front()->symbols.end(); ++iter2)
      {
        symbols.push_front(*iter2);
      }
      for(std::list<EG*>::iterator iter2 = (*iter)->cuts.front()->cuts.begin();
        iter2 != (*iter)->cuts.front()->cuts.end(); ++iter2)
      {
        cuts.push_front(*iter2);
      }

      (*iter)->cuts.front()->cuts.clear();
      delete (*iter);
      iter = cuts.erase(iter);
    }
    else if(!(*iter)->open())
    {
      delete (*iter);
      iter = cuts.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

char EG::find_symbol()
{
  if(symbols.size() == 0)
  {
    for(std::list<EG*>::iterator iter = cuts.begin();
      iter != cuts.end(); ++iter)
    {
      if((*iter)->cuts.size() == 0 && (*iter)->symbols.size() == 1)
      {
        return (*iter)->find_symbol();
      }
    }

    for(std::list<EG*>::iterator iter = cuts.begin();
      iter != cuts.end(); ++iter)
    {
      char c;
      c = (*iter)->find_symbol();
      if(c != '\0')
      {
        return c;
      }
    }
    return '\0';
  }
  return symbols.front();
}

bool EG::check_consistency(int d)
{
  simplify();
  print(d);
  if(finished())
  {
    return true;
  }
  if(!open())
  {
    return false;
  }

  char c = find_symbol();
  EG g1 = EG(*this);
  EG g2 = EG(*this);
  g1.reduce(c,true);
  g2.reduce(c, false);

  return g1.check_consistency(d+1) || g2.check_consistency(d+1);
}
