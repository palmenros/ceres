#![warn(
    clippy::pedantic,
    // clippy::nursery,
    // restriction
    clippy::alloc_instead_of_core,
    clippy::as_underscore,
    clippy::assertions_on_result_states,
    clippy::clone_on_ref_ptr,
    clippy::create_dir,
    clippy::dbg_macro,
    clippy::decimal_literal_representation,
    clippy::default_union_representation,
    clippy::deref_by_slicing,
    clippy::else_if_without_else,
    clippy::empty_drop,
    clippy::empty_structs_with_brackets,
    clippy::exit,
    clippy::expect_used,
    clippy::filetype_is_file,
    // clippy::float_arithmetic,
    clippy::float_cmp_const,
    clippy::fn_to_numeric_cast_any,
    clippy::format_push_string,
    clippy::get_unwrap,
    clippy::if_then_some_else_none,
    clippy::let_underscore_must_use,
    clippy::lossy_float_literal,
    clippy::map_err_ignore,
    clippy::mem_forget,
    clippy::mixed_read_write_in_expression,
    clippy::modulo_arithmetic,
    clippy::mutex_atomic,
    clippy::non_ascii_literal,
    clippy::panic,
    clippy::partial_pub_fields,
    // clippy::print_stderr,
    // clippy::print_stdout,
    clippy::rc_buffer,
    clippy::rc_mutex,
    clippy::rest_pat_in_fully_bound_structs,
    clippy::same_name_method,
    clippy::self_named_module_files,
    clippy::shadow_unrelated,
    // clippy::std_instead_of_alloc,
    // clippy::std_instead_of_core,
    clippy::str_to_string,
    clippy::string_add,
    clippy::string_slice,
    clippy::string_to_string,
    clippy::todo,
    clippy::try_err,
    clippy::unimplemented,
    clippy::unnecessary_self_imports,
    clippy::unneeded_field_pattern,
    clippy::unseparated_literal_suffix,
    // clippy::use_debug,
    clippy::verbose_file_reads,
    // clippy::indexing_slicing,
    // clippy::unwrap_used,
    // clippy::integer_division,
  )]
#![allow(
    clippy::missing_errors_doc,
    clippy::missing_panics_doc,
    clippy::missing_safety_doc,
    clippy::similar_names,
    clippy::struct_excessive_bools,
    clippy::verbose_bit_mask
)]

use crate::trans_unit::TransUnit;
use clap::{Arg, Command};
use pest::Parser;
use std::{fs, path::PathBuf};

mod expr;
mod stmt;
mod top_level;
mod trans_unit;

#[derive(pest_derive::Parser)]
#[grammar = "ceres.pest"]
pub struct CeresParser;

fn main() -> anyhow::Result<()> {
    let args = Command::new("ceresc")
        .arg(Arg::new("file").required(true))
        .get_matches();

    let path = PathBuf::from(args.get_one::<String>("file").unwrap());
    let contents = fs::read_to_string(path)?;

    match CeresParser::parse(Rule::trans_unit, &contents) {
        Ok(pairs) => {
            println!("Parsed: {:#?}", TransUnit::parse(pairs));
        }
        Err(e) => {
            eprintln!("Parse failed: {e:?}");
        }
    }

    Ok(())
}
