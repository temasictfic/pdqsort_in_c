struct CopyonDrop
{
    int *src;
    int *dest;
}
void drop(struct CopyonDrop *self)
{
    memmove(self->src, self->dest, 1);
}

void shift_head(int v[], int (*is_less)(int*, int*))
{
    struct CopyonDrop hole;
    int len = *(&v + 1) - v;

    if (len >= 2 && is_less(v[1], v[0]))
    {
        int *tmp = &v[0];
        hole.src = &*tmp;
        hole.dest = v + 1;
        memmove(v + 1, v, 1);

        for (size_t i = 2; i < len; i++)
        {
            if (!is_less(&*(v + i), &*tmp))
                break;
            memmove(v + i, v + i - 1, 1);
            hole.dest = v + i;
        }
        
    }
}

void shift_tail(int v[], int (*is_less)(int*, int*))
{
    struct CopyonDrop hole;
    int len = *(&v + 1) - v;
    if (len >= 2 && is_less(v[len - 1], v[len - 2]))
    {
        int *tmp = &v[len -1];
        hole.src = &*tmp;
        hole.dest = v + len - 2;
        memmove(v + len - 2, v + len - 1, 1);

        for (size_t i = len - 3; i >= 0; i--)
        {
            if (!is_less(&*tmp, &*(v+i)))
                break;
        }
        memmove(v + i, v + i + 1, 1);
        hole.dest = v + i;

    }
}




int partial_insertion_sort(int v[], int (*is_less)(int*, int*))
{
    const size_t MAX_STEPS = 5;
    const size_t SHORTEST_SHIFTING = 50;

    int len = *(&v + 1) - v;
    int i = 1;

    for (size_t i = 0; i < MAX_STEPS; i++)
    {
        while (i < len && !is_less(v[i], v[i-1]))
            i += 1;    
        if (i == len)
            return(1);
        if (len < SHORTEST_SHIFTING)
            return (0);
        swap_arr(v, i-1 , i);
        shift_tail(&v[..i], is_less);
        shift_head(&v[i..], is_less);
    }
    return(0);
}



void insertion_sort(int v[], int (*is_less)(int*, int*))
{
    // arr_len = *(&v + 1) - v;
    for (size_t i = 1; i < *(&v + 1) - v; i++)
        shift_tail(&v[..i + 1], is_less);
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
            if (child + 1 < 5 && is_less(&v[child], &v[child + 1]))
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



size_t partition_in_blocks(int *v, size_t *pivot, int (*is_less)(int*, int*))
{
    const size_t BLOCK = 128;
    unsigned char offsets_l[BLOCK];
    unsigned char offsets_r[BLOCK];

    int *l = v;
    size_t block_l = BLOCK;
    int *start_l = NULL;
    int *end_l = NULL;

    int *r = l + *(&v + 1) - v;
    size_t block_r = BLOCK;
    int *start_r = NULL;
    int *end_r = NULL;
     

    size_t width(int *l, int *r)
    {
        return (l - r) / sizeof(int);

    }
    while (1)
    {
        int is_done;
        is_done = width(l, r) <= 2 * BLOCK;

        if (is_done)
        {
            size_t rem;
            rem = width(l, r);
            if (start_l < end_l)
                block_r = rem;
            else if (start_r < end_r)
                block_l = rem;
            else
            {
                block_l = rem / 2;
                block_r = rem - block_l;
            }
        }
        if(start_l == end_l)
        {
            start_l = offsets_l;
            end_l = start_l;
            int *elem = l;

            for (size_t i = 0; i < block_l; i++)
            {
                *end_l = (unsigned char)i;
                end_l = offset(end_l, (long)!is_less( &*elem, pivot));
                elem = offset(elem, 1);
            }
            
        }
        if(start_r == end_r)
        {
            start_r = offsets_r;
            end_r = start_r;
            int *elem = r;

            for (size_t i = 0; i < block_r; i++)
            {
                elem = offset(elem, -1);
                *end_r = (unsigned char)i;
                end_e = offset(end_r, (long)is_less( &*elem, pivot));
            }
            
        }

        size_t count = min(width(start_l, end_l), width(start_r, end_r));

        if (count > 0)
        {
            int tmp = offset(l , (long)*start_l);
            memmove(offset(r, -(long)*start_r - 1), offset(l , (long)*start_l), 1);

            for (size_t i = 1; i < count; i++)
            {
                start_l = offset(start_l, 1);
                memmove(offset(l , (long)*start_l), offset(r, -(long)*start_r - 1), 1);
                start_r = offset(start_r, 1);
                memmove(offset(r, -(long)*start_r - 1), offset(l , (long)*start_l), 1);
                
            }
            memmove(&tmp, offset(r, -(long)*start_r - 1), 1);
            free(tmp);
            start_l = offset(start_l, 1);
            start_r = offset(start_r, 1);
            
        }
        if (start_l == end_l)
            l = offset(l, (long)block_l);
        if (start_t == end_r)
            r = offset(r, -(long)block_r);
        if (is_done)
            break;
    }
    if (start_l < end_l)
    {
        while(start_l < end_l)
        {
            end_l = offset(end_l, -1);
            swap(offset(l, (long)*end_l), offset(r, -1));
            r = offset(r, -1);
        }
        return(width(v, r));
    }
    else if (start_r < end_r)
    {
        while (start_r < end_r)
        {
            end_r = offset(end_r, -1);
            swap(l, offset(r, -(long)(*end_r) - 1));
            l = offset(l, 1);
        }
        return(width(v, l));
    }
    else
        return(width(v, l));

}



struct choose_pivot partition(int *v, size_t pivot, int (*is_less)(int*, int*))
{
    struct split_at at;
    struct choose_pivot cp;
    struct CopyonDrop _pivot_guard;
    size_t l, r;

    swap_arr(v, 0, pivot);
    at = split_at(v, 1);
    pivot = at.left;
    v = at.right;
    size_t *pivot = &pivot[0];

    tmp = pivot;
    _pivot_guard.src = &*tmp;
    _pivot_guard.dest = pivot;
    size_t *pivot = &*tmp;

    l = 0;
    r = *(&v + 1) - v;


    while (l < r && is_less(v[l], pivot))
        l += 1;
    while (l < r && !is_less(v[r - 1], pivot))
        r -= 1;
    
    mid = l + partition_in_blocks(&v[l..r], pivot, is_less);
    was_partitioned = l >= r;

    swap_arr(v, 0, mid);

    cp.pivot = mid;
    cp.likely_sorted = was_partitioned;
    return (cp);
}





size_t partition_equal(int *v, size_t pivot, int (*is_less)(int*, int*))
{
    struct split_at at;
    struct CopyonDrop _pivot_guard;
    size_t l, r;

    swap_arr(v, 0, pivot);
    at = split_at(v, 1);
    size_t *pivot = at.left;
    v = at.right;
    size_t *pivot = &pivot[0];

    tmp = pivot;
    _pivot_guard.src = &*tmp;
    _pivot_guard.dest = pivot;
    size_t *pivot = &*tmp;

    l = 0;
    r = *(&v + 1) - v;
    while(1)
    {
        while (l < r && !is_less(pivot, v[l]))
            l += 1;
        while (l < r && is_less(pivot, v[r - 1]))
            r -= 1;
        if (l >= r)
            break;
        
        r -= 1;
        swap(v + l, v + r);
        l += 1;
    }

    return (l + 1);

}




void break_patterns(int *v)
{
    size_t len, modulus, pos, other;
    unsigned int random;

    len = *(&v + 1) - v;
    if (len >= 8)
    {
        random = (unsigned int)len;
        unsigned int gen_uint()
        {
            random ^= random << 13;
            random ^= random >> 17;
            random ^= random << 5;
            return(random);
        }
        size_t gen_sizet()
        {
            if ((sizeof(size_t) << 3) <= 32)
               return (gen_uint());
            else
                return(((unsigned long)gen_uint()) << 32 | (size_t)((unsigned long)gen_uint()))
        }

        modulus = next_power_of_two(len);
        pos = len / 4 * 2;

        for (size_t i = 0; i < 3; i++)
        {
            other = gen_sizet() & (modulus - 1);
            if (other >= len)
                other -= len;
            swap_arr(v, pos - 1 + i, other);
        }
        
    }
}







struct choose_pivot choose_pivot(int* v, int (*is_less)(int*, int*))
{
    size_t SHORTEST_MEDIAN_OF_MEDIANS = 50;
    size_t MAX_SWAPS = 4 * 3 ;
    int swaps;
    size_t len;
    size_t a,b,c;
    struct choose_pivot cp;
    //????void (*sort2)(size_t, size_t) sort2;

    len = *(&v + 1) - v;

    a = len / 4 * 1;
    b = len / 4 * 2;
    c = len / 4 * 3;

    swaps = 0;

    if (len >= 8)
    {
        void sort2(size_t *a, size_t *b)
        {
            if (is_less(v[*b], v[*a]))
            {
                swap(a, b);
                swaps += 1;
            }
        }

        void sort3(size_t *a, size_t *b, size_t *c)
        {
            sort2(a, b);
            sort2(a, c);
            sort2(a, b);
        }
        
        if (len >= SHORTEST_MEDIAN_OF_MEDIANS)
        {
            void sort_adjacent(size_t *a)
            {
                size_t tmp;
                tmp = *a;
                sort3(&(tmp - 1), a, &(tmp + 1));
            }

            sort_adjacent(&a);
            sort_adjaent(&b);
            sort_adjacent(&c);
        }
        sort3(&a, &b, &c);
    }

    if (swaps < MAX_SWAPS)
    {
        b = 0;
        swaps = 0;
    }
    else{
        reverse(v);
        cp.pivot = len - 1 - b;
        cp.likely_sorted = 1;
        return (cp);
    }
}

void	recurse(int *v, int (*is_less)(int*, int*), int *pred, unsigned int limit)
{
	size_t MAX_INSERTION = 20;
	size_t len;
	int was_balanced = 1;
	int was partitioned = 1;
	int mid;
	int pivot;
	struct choose_pivot cp;
	struct partition part;
	struct split_at at1;
	struct split_at at2;

	while (1)
	{
		len = *(&v + 1) - v;
		if (len <= MAX_INSERTION)
		{
			insertion_sort(v, is_less);
			return ;
		}

		if (limit == 0)
		{
			heap_sort(v, is_less);
			return ;
		}

		if
			!was_balanced
			{
				break_patterns(v);
				limit -= 1;
			}

		cp = choose_pivot(v, is_less);
		pivot = cp.pivot;
		likely_sorted = cp.likely_sorted;

		if (was_balanced && was_partitioned && likely_sorted)
		{
			if (partial_insertion_sort(v, is_less))
				return ;
		}

		if (*pred)
		{
			if (!is_less(pred, &v[pivot]))
			{
				mid = partition_equal(v, pivot, is_less);

				v = &v[mid..];
                continue ;
			}
		}

		part = partiton(v, pivot, is_less);
		mid = part.mid;
		was_p = part.was_p;

		at1 = split_at(v, mid);
		left = at1.left;
		right = at1.right;

		at2 = split_at(right, 1);
		pivot = at2.left;
		right = at2.right;
		pivot = pivot[0];

		if (*(&left + 1) - left < *(&right + 1) - right)
		{
			recurse(left, is_less, pred, limit);
			v = right;
			pred = pivot;
		}
		else
		{
			recurse(right, is_less, pivot, limit);
			v = left;
		}
	}
}


void	quicksort(int *v, int (*is_less)(int*, int*))
{
	size_t	limit;

	limit = (sizeof(size_t)<< 3) - leading_zeros(*(&v + 1) - v);
	recurse(v, is_less, NULL, limit);
}

void partition_at_index_loop(int v[], size_t index, int (*is_less)(int*, int*), int* pred)
{
    struct chose_pivot cp;
    struct partition part;
    struct split_at at1, at2;
    while(1)
    {
        const size_t MAX_INSERTION = 10;
        if (*(&v + 1) - v <= MAX_INSERTION)
        {
            insertion_sort(v, is_less);
            return;
        }

        cp = choose_pivot(v, is_less);
        int pivot = cp.pivot;

        if (*pred)
        {
            if (!is_less(pred, &v[pivot]))
            {
                size_t mid = partition_equal(v, pivot, is_less);

                if (mid > index)
                    return;
                v = &v[mid..];
                index = index - mid;
                pred = NULL;
                continue;
            }
        }
        part = partition(v, pivot, is_less);
        size_t mid = part.mid;

        at1 = split_at(v, mid);
        int *left = at1.left;
        int *right = at1.right;

        at2 = split_at(right, 1);
        int *pivot = at2.left;
        int *right = at2.right;
        int *pivot = &pivot[0];

        if (mid < index)
        {
            v = right;
            index = index - mid -1;
            pred = &pivot;
        }
        else if (mid > index)
        {
            v = left;
        }
        else 
        {
            return;
        }
    }
}

struct part_at_index partition_at_index(int v[], size_t index, int (*is_less)(int*, int*))
{
    struct part_at_index pai;
    struct find_max_index ma;
    struct find_min_index mi;
    struct split_at at1, at2;

    if (index >= *(&v + 1) - v)
        write(1,"error", 5);
    if (index == *(&v + 1) - v - 1)
    {
        ma = find_max_index(v, max);
        max_index = ma.max_index;
        swap_arr(v, max_index, index);
    }
    else if ( index == 0)
    {
        mi = find_min_index(v, min);
        min_index = mi.min_index;
        swap_arr(v, min_index, index);
    }
    else
    {
        partition_at_index_loop(v, index, is_less, NULL);
    }
    at1 = split_at(v, index);
    int *left = at1.left;
    int *right = at1.right;

    at2 = split_at(right, 1);
    int *pivot = at2.left;
    int *right = at2. right;
    int *pivot = &pivot[0];

    pai.left = left;
    pai.pivot = pivot;
    pai.right = right;
    return(pai);
}
