use kernel::bindings::list_head;


impl IRM {
    static mut prev_module: list_head = unsafe { core::mem::zeroed() }; 

    fn hide(_module: &'static ThisModule) -> Result<Self> {
        Ok(IRM)
    }
}
