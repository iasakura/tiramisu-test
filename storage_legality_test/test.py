import islpy as isl

# This is an example which checks the legality of the following computation & scheduling which computes matrix vector multiplication iteratively.
# for (t)
#   for (j)
#     for (i)
#       prod[t%2][i]= prod[t%2][i] + A[i][j] * prod[(t-1)%2][j]
# Tiramisu code:
# computation prod("prod" {t, i, j}, p_float32);
# prod.set_expression(prod(t, i, j - 1) + A(i, j) * prod(t - 1, j, N - 1));
# prod.interchange(i, j)
# prod.store_in("prod_buf", {t % 2, i, 0})

# Iteration domain
I = isl.Set('[T, N] -> {prod[t, i, j] : 0 <= t < T and 0 <= i < N and 0 <= j < N}')
# Read relation
R = isl.Map('[T, N] -> {prod[t, i, j] -> prod[t, i, j-1] : t > 0 and j > 0; prod[t, i, j] -> prod[t-1, j, N-1] : t > 0 and j > 0}').intersect_domain(I)
# Default write relation
Wdefault = isl.Map('[T, N] -> {prod[t, i, j] -> prod[t, i, j]}').intersect_domain(I)
# User specified write function
W = isl.Map('[T, N, M] -> {prod[t, i, j] -> prod_buf[t % 2, i, 0]}').intersect_domain(I)

# Scheduling function & relation
S = isl.Map('[T, N] -> {prod[t, i, j] -> [t, j, 0]}')
O = S.lex_lt_union_map(S)

# Dependency relation
D = Wdefault.apply_range(R.reverse())

# Check legality of time scheduling
is_time_sched_ok = D.is_subset(O)
print(is_time_sched_ok)

# Last use function
# {(i->j)->k : i->j \in D and j -> k \in S }.lexmax()
L = D.range_map().apply_range(S).lexmax().domain().unwrap()

# Conflict set CS = C1 \cap C2
# C1 = { i1 -> i2 : S(i1) < S(L(i2)) }
C1 = S.lex_lt_union_map(L.apply_range(S))
# C2 = { i1 -> i2 : S(i2) < S(L(i1)) }
#    = C.reverse()
C2 = C1.reverse()
# CS = { i1 -> i2 : i1 < L(i2) && i2 < L(i1) }
#    = { i1 -> i2 : i1 < L(i2) } \cap { i1 -> i2 : i2 < L(i1) }
#    = C1 \cap C2
CS = C1.intersect(C2).subtract(I.identity())

# pairs of iterations which writes to same indices
same_idx=W.domain_product(W).domain().unwrap()
# { i1 -> i2 : i1 -> i2 \in CS, WW(i1) == WW(i2) }
invalid=same_idx.intersect(CS)
is_memory_sched_ok=invalid.is_empty()

print(is_memory_sched_ok)
