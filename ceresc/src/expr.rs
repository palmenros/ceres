use crate::Rule;
use pest::iterators::Pairs;
use pest::pratt_parser::PrattParser;

lazy_static::lazy_static! {
    static ref PRATT_PARSER: PrattParser<Rule> = {
        use pest::pratt_parser::{Assoc::Left, Op};
        use Rule::{add, divide, modulo, multiply, subtract, unary_minus, unary_not, eqeq};

        // Precedence is defined lowest to highest
        PrattParser::new()
            .op(Op::infix(eqeq, Left))
            .op(Op::infix(add, Left) | Op::infix(subtract, Left))
            .op(Op::infix(multiply, Left) | Op::infix(divide, Left) | Op::infix(modulo, Left))
            .op(Op::prefix(unary_minus) | Op::prefix(unary_not))
    };
}

#[derive(Debug, Clone, Copy)]
pub enum UnaryOpTy {
    Minus,
    Neg,
}

#[derive(Debug, Clone, Copy)]
pub enum BinOpTy {
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    EqEq,
}

#[derive(Debug)]
pub enum Expr {
    Ident(String),
    Integer(String),
    FnCall {
        id: String,
        args: Vec<Expr>,
    },
    UnaryOp {
        op: UnaryOpTy,
        ex: Box<Expr>,
    },
    BinOp {
        lhs: Box<Expr>,
        op: BinOpTy,
        rhs: Box<Expr>,
    },
}

impl Expr {
    #[must_use]
    pub fn parse(pairs: Pairs<Rule>) -> Expr {
        PRATT_PARSER
            .map_primary(|primary| match primary.as_rule() {
                Rule::fn_call => {
                    let mut ps = primary.into_inner();
                    let id = ps.next().unwrap().as_str().to_owned();
                    let args = ps
                        .next()
                        .unwrap()
                        .into_inner()
                        .map(|p| Expr::parse(p.into_inner()))
                        .collect();
                    Expr::FnCall { id, args }
                }
                Rule::ident => Expr::Ident(primary.as_str().to_owned()),
                Rule::integer => Expr::Integer(primary.as_str().to_owned()),
                Rule::expr => Self::parse(primary.into_inner()),
                rule => unreachable!("Expr::parse expected atom, found {:?}", rule),
            })
            .map_infix(|lhs, op, rhs| {
                let op = match op.as_rule() {
                    Rule::add => BinOpTy::Add,
                    Rule::subtract => BinOpTy::Subtract,
                    Rule::multiply => BinOpTy::Multiply,
                    Rule::divide => BinOpTy::Divide,
                    Rule::modulo => BinOpTy::Modulo,
                    Rule::eqeq => BinOpTy::EqEq,
                    rule => unreachable!("Expr::parse expected infix operation, found {:?}", rule),
                };
                Expr::BinOp {
                    lhs: Box::new(lhs),
                    op,
                    rhs: Box::new(rhs),
                }
            })
            .map_prefix(|op, rhs| {
                let op = match op.as_rule() {
                    Rule::unary_minus => UnaryOpTy::Minus,
                    Rule::unary_not => UnaryOpTy::Neg,
                    _ => unreachable!(),
                };
                Expr::UnaryOp {
                    op,
                    ex: Box::new(rhs),
                }
            })
            .parse(pairs)
    }
}
