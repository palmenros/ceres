use crate::expr::{self, Expr};
use crate::Rule;
use pest::iterators::Pairs;

#[derive(Debug)]
pub enum Ty {
    U8,
    U16,
    U32,
    I8,
    I16,
    I32,
    Bool,
}

impl Ty {
    #[must_use]
    pub fn parse(mut pairs: Pairs<Rule>) -> Ty {
        match pairs.next().unwrap().as_rule() {
            Rule::tyi8 => Ty::I8,
            Rule::tyi16 => Ty::I16,
            Rule::tyi32 => Ty::I32,
            Rule::tyu8 => Ty::U8,
            Rule::tyu16 => Ty::U16,
            Rule::tyu32 => Ty::U32,
            Rule::tybool => Ty::Bool,
            rule => unreachable!("entered rule: {rule:?}"),
        }
    }
}

#[derive(Debug)]
pub struct VarDecl {
    id: String,
    ty: Ty,
}

impl VarDecl {
    #[must_use]
    pub fn parse(mut pairs: Pairs<Rule>) -> VarDecl {
        let id = pairs.next().unwrap().as_str().to_owned();
        let ty = Ty::parse(pairs);
        VarDecl { id, ty }
    }
}

#[derive(Debug)]
pub struct Block {
    block: Vec<Stmt>,
}

impl Block {
    #[must_use]
    pub fn parse(pairs: Pairs<Rule>) -> Block {
        Block {
            block: pairs.map(|p| Stmt::parse(p.into_inner())).collect(),
        }
    }
}

#[derive(Debug)]
pub enum Stmt {
    Expr {
        expr: Expr,
    },
    VarDef {
        decl: VarDecl,
        assign: Option<Expr>,
    },
    Block {
        block: Block,
    },
    Return {
        expr: Expr,
    },
    IfElse {
        cond: Expr,
        ifb: Block,
        elseb: Option<Block>,
    },
}

impl Stmt {
    #[must_use]
    pub fn parse(mut pairs: Pairs<Rule>) -> Stmt {
        let pair = pairs.next().unwrap();
        match pair.as_rule() {
            Rule::block_stmt => Stmt::Block {
                block: Block::parse(pair.into_inner()),
            },
            Rule::expr => Self::Expr {
                expr: Expr::parse(pair.into_inner()),
            },
            Rule::var_decl_stmt => {
                let mut ps = pair.into_inner();
                let decl = VarDecl::parse(ps.next().unwrap().into_inner());
                let assign = ps.next().map(|p| Expr::parse(p.into_inner()));
                Stmt::VarDef { decl, assign }
            }
            Rule::ret_stmt => Stmt::Return {
                expr: Expr::parse(pair.into_inner()),
            },
            Rule::if_stmt => {
                let mut ps = pair.into_inner();
                let cond = Expr::parse(ps.next().unwrap().into_inner());
                let ifb = Block::parse(ps.next().unwrap().into_inner());
                let elseb = ps.next().map(|p| Block::parse(p.into_inner()));
                Stmt::IfElse { cond, ifb, elseb }
            }
            rule => unreachable!("entered rule: {rule:?}"),
        }
    }
}
