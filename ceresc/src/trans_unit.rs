use crate::top_level::TopLevel;
use crate::Rule;
use pest::iterators::Pairs;

#[derive(Debug)]
pub enum TransUnit {
    Defs { list: Vec<TopLevel> },
}

impl TransUnit {
    #[must_use]
    pub fn parse(mut pairs: Pairs<Rule>) -> TransUnit {
        let pair = pairs.next().unwrap();
        match pair.as_rule() {
            Rule::trans_unit => {
                let mut list = Vec::new();
                for p in pair.into_inner() {
                    let ps = p.into_inner();
                    if ps.peek().is_some() {
                        list.push(TopLevel::parse(ps));
                    }
                }

                TransUnit::Defs { list }
            }
            rule => unreachable!("entered rule: {rule:?}"),
        }
    }
}
