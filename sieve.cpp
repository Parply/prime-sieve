#include <cstdio>
#include <iostream>
#include <ostream>

#include <vector>
#include <omp.h>
#include <math.h>




void eratosthenesSieve(unsigned long long int n, std::vector<unsigned long long int> & result
)
{
	std::vector<int> a(n,1);
	unsigned long long int lim=sqrt(n);
	a[0]=0;
	a[1]=0;
	#pragma omp parallel
	{
	#pragma omp single
	for (unsigned long long int i=0;i<=lim;i++)
		if (a[i])
		{
			result.push_back(i);
			#pragma omp parallel for shared(a,i) schedule(static)
			for (unsigned long long int j=i*i;j<=n;j+=i)
			{
				
				a[j] =0;
			}
				
			
		}
	std::vector<unsigned long long int> slave;
	#pragma omp for nowait schedule(static)  	
for (unsigned long long int k=lim+1;k<=n;k++)
	{
		if (a[k])
			slave.push_back(k);
	}
	#pragma omp for schedule(static) ordered
	for (int i=0;i<omp_get_num_threads();i++)
	{
	#pragma omp ordered
		result.insert(result.end(),slave.begin(),slave.end());
	}
	
	
		
	}
}
unsigned long long int eratosthenesSieveCount(unsigned long long int n)
{
	std::vector<int> a(n+1,1);
	unsigned long long int lim=sqrt(n);
	a[0] =0;
	a[1] =0;
	unsigned long long int result=0;
	#pragma omp parallel
	{
	#pragma omp single
	for (unsigned long long int i=0;i<=lim;i++)
		if (a[i])
		{
			#pragma omp parallel for shared(a) schedule(static)
			for (unsigned long long int j=i*i;j<=n;j+=i)
			{
				
				a[j] =0;
			}
				
			
		}
	#pragma omp for reduction(+:result)	
	for (unsigned long long int k=0;k<=n+1;k++)
	{
		result+= a[k];
	}
	
	
	
		
	}
	return result;
}


int main()
{
	unsigned long long int n;
	char choice1,choice2;
	std::cout << "Compute primes up to:" << std::endl;
	std::cin >> n;
	std::cout << "List?" <<std::endl;
	std::cin >> choice1;
	if (choice1=='y')
	{
		std::vector<unsigned long long int>  result;
		eratosthenesSieve(n,result);
		std::cout << "Primes less than " << n << std::endl ;
		for (auto i:result)
			std::cout << i << " ";
		std::cout <<std::endl;
	} 
	std::cout << "Count?" <<std::endl;
	std::cin >> choice2;

	if (choice2=='y')
	{
		unsigned long long int result=eratosthenesSieveCount(n);
		std::cout << result << " primes less than " << n << std::endl ;

	}
	return 0;
}
