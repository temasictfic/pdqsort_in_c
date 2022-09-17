#include <stdio.h>
int is_less(int *a, int *b)
{
    return(*a < *b);
}
void swap_arr(int v[], int node, int child)
{
    int *pnode = &v[node];
    int *pchild = &v[child];
    
    void	ft_swap(int *a, int *b)
    {
    	int	mid;
    
    	mid = *a;
    	*a = *b;
    	*b = mid;
    }
    ft_swap(pnode, pchild);
}

void heap_sort(int v[], int (*is_less)(int*, int*))
{
    void sift_down(int v[], int node)
    {
        while(1)
        {
            int child = 2 * node + 1;
            if (child >= 5)
                break;
            if (child + 1 < (5) && is_less(&v[child], &v[child + 1]))
                child + 1;
            if (!is_less(&v[node], &v[child]))
                break;
            swap_arr(v, node, child);
            node = child;
        }
    }
    
    void sift_down2(int v[], int node, int i)
    {
        while(1)
        {
            int child = 2 * node + 1;
            if (child >= i)
                break;
            if (child + 1 < i && is_less(&v[child], &v[child + 1]))
                child + 1;
            if (!is_less(&v[node], &v[child]))
                break;
            swap_arr(v, node, child);
            node = child;
        }
    }
    for (size_t i = (5 / 2) - 1; i >= 0 ; i--)
    {
        sift_down(v, i);
    }
    for (size_t i = 5 - 1; i >= 1 ; i--)
    {
        swap_arr(v, 0, i);
        sift_down2(v, 0, i);
    }
}
int main()
{
   
    int v[] = {-5, 4, 1, -3, 2};
    heap_sort(v, is_less);
    for (size_t i = 0; i < 5 ; i++)
    {
        printf("%d\n", v[i]);
    }

    return (0);
}