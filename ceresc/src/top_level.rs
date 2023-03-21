use crate::{
    stmt::{Block, Stmt, Ty, VarDecl},
    Rule,
};
use pest::iterators::Pairs;

#[derive(Debug)]
pub enum TopLevel {
    FnDef {
        id: String,
        args: Vec<VarDecl>,
        ret: Option<Ty>,
        block: Block,
    },
}

impl TopLevel {
    #[must_use]
    pub fn parse(mut pairs: Pairs<Rule>) -> TopLevel {
        let pair = pairs.next().unwrap();
        match pair.as_rule() {
            Rule::fn_def_top => {
                let mut ps = pair.into_inner();
                let id = ps.next().unwrap().as_str().to_owned();
                let args = ps
                    .next()
                    .unwrap()
                    .into_inner()
                    .map(|p| VarDecl::parse(p.into_inner()))
                    .collect();

                let ret_ps = ps.next().unwrap().into_inner();
                let ret = ret_ps.peek().is_some().then(|| Ty::parse(ret_ps));
                let block = Block::parse(ps.next().unwrap().into_inner());

                Self::FnDef {
                    id,
                    args,
                    block,
                    ret,
                }
            }
            rule => unreachable!("entered rule: {rule:?}"),
        }
    }
}
