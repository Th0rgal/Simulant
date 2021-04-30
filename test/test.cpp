#include <iostream>
#include <vector>


void	test2(std::vector<std::vector<int>> b) {
	b[0][0] = 21;
}

int	main(void) {
	std::vector<std::vector<int>> a = {{3, 3, 3}, {3, 3, 3}, {3, 3, 3}};
	std::vector<std::vector<int>> b(a);

	b[0][0] = 42;

	std::cout << "B = " << b[0][0] << ", A = " << a[0][0] << std::endl;

	test2(a);
	
	std::cout << "A = " << a[0][0] << std::endl;

	return (0);
}
