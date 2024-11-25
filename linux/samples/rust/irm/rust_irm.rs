//! Innocent Rusty Module
use kernel::prelude::*;

module!{
    type: IRM,
    name: "irm",
    license: "GPL",
}


struct IRM;

impl file::Operations for IRM {
    fn open(context: &Self::OpenData, file: &file::File) -> Result<Self::Data> {
        pr_info!("File was opened\n");
        Ok(())
    }
}

impl kernel::Module for IRM {
    fn init(_module: &'static ThisModule) -> kernel::error::Result<Self> {
        pr_info!("Hello World\n");
        Ok(IRM)
    }
}
