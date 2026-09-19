use std::env;
use std::process;
use std::fs;

struct Config {
    query: String,
    filename: String,
}
impl Config{
    /// Constructor for config struct
    /// args must be Vec<String> reference and maintain the ownership
    ///
    fn build(args: &[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("try: mingrep <pattern> <filename>");
        }
        Ok(Config {query: args[1].clone(), filename: args[2].clone()})
    }

}
fn main() {
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 3 {
        panic!("error deberian ser 3 argumentos");
    }
    let k = &args[1];
    let fname = &args[2];

    let ftext = fs::read_to_string(fname)
        .unwrap_or_else(|e| { 
            println!("Error no se pudo leer/abrir el archivo quizas nop existe fallo el sistema {:?}", e); 
            process::exit(1);
        });


}
