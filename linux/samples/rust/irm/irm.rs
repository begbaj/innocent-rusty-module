//! Innocent Rusty module
use kernel::prelude::*;

module!{
    type: IRM,
    name: "irm",
    author: "Began Bajrami",
    description: "just a chill module wanting to create a reverse shell on your kernel",
    license: "GPL",
}

struct IRM;

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> kernel::error::Result<Self> {
        pr_info!("IRM activated\n");
        Ok(IRM)
    }
}

// for now, this is just useless, since the "module" is built-in. I will proceed to make it
// modularized afterwards, since I really to not need right now it to be loadable/unloadable
impl Drop for IRM {
    fn drop(&mut self){
        pr_info!("Why? Nobody likes me, I'm an Innocent chill module ;( \n");
    }
}
