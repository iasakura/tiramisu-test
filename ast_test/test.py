import islpy as isl

def printASTAsC(ast):
  p = isl.Printer.to_str(ast.get_ctx())
  p = p.set_output_format(isl.format.C)
  p = p.print_ast_node(ast)
  print(p.get_str())

def dump_info(node, build):
    print('Called!!!')
    print(node)
    print(node.user_get_expr())
    print(build.get_schedule())
    return node

I = isl.UnionSet('[n] -> {S[i, j] : 0 <= i < n and 0 <= j < n; T[i] : 0 <= i < n}')
S = isl.UnionMap('[n] -> {S[i, j] -> [0, i+j, j]; T[i] -> [1, i, 0]}').intersect_domain(I)

build = isl.AstBuild.alloc(isl.DEFAULT_CONTEXT)
(build, _) = build.set_at_each_domain(dump_info)

ast = build.node_from_schedule_map(S)
printASTAsC(ast)
