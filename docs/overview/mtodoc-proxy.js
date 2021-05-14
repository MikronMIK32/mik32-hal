import { readdirSync, lstatSync, fstat, mkdirSync, readFileSync, writeFileSync } from 'fs';
import { execSync } from 'child_process';

const ignoreNames = [
    '.git',
    'node_modules',
    'dest'
];

const dest = '../dest/';

function listFiles(root, predicate) {
    if (!predicate) predicate = (f) => f.substr(-3) === '.md';

    const q = [root];
    const mdFiles = {};

    while (q.length > 0) {
        let cd = q.shift();
        const files = readdirSync(cd);

        for (const file of files) {
            if (ignoreNames.includes(file))
                continue;

            if (lstatSync(cd + file).isDirectory()) {
                q.push(cd + file + '/');
            } else if (predicate(file)) {
                let outDir = (cd+file).substr(root.length);
                outDir = outDir.substr(0, outDir.lastIndexOf('/')+1);
                mdFiles[cd+file] = outDir;
            }
        }
    }

    return mdFiles;
}

const mdFiles = listFiles('../../');
const dirs = Object.values(mdFiles).map(e=>dest+e);

console.log('Creating dirs:', dirs);

for (const dir of dirs) {
    try {
        mkdirSync(dir);
    } catch(_) {

    }
}

const cmd = Object.keys(mdFiles)
    .map(path=>{
        console.log(path,mdFiles[path]);
        return `yarn run mdtodoc ${path} --layout \"assets/layouts/page.html\" --dest \"${dest+mdFiles[path]}\"`
    })
    .join(' && ');
    
console.log(cmd);
execSync(cmd);

const htmlFiles = Object.keys(listFiles(dest, f=>f.substr(-5) === '.html'));

for (const file of htmlFiles) {
    let content = readFileSync(file, {
        encoding: 'utf-8'
    });

    let step = 0;

    console.log(file);

    const hrefs = [];
    let href = '';

    for (let i = 0; i < content.length; i++) {
        if (step === 0) {
            if (content.charAt(i) === 'h') {
                step++;
            } else {
                step = 0;
            }
        }
        else if (step === 1) {
            if (content.charAt(i) === 'r') {
                step++;
            } else {
                step = 0;
            }
        }
        else if (step === 2) {
            if (content.charAt(i) === 'e') {
                step++;
            } else {
                step = 0;
            }
        }
        else if (step === 3) {
            if (content.charAt(i) === 'f') {
                step++;
            } else {
                step = 0;
            }
        }
        else if (step === 4) {
            if (content.charAt(i) === '=') {
                step++;
            } else {
                step = 0;
            }
        }
        else if (step === 5) {
            if (content.charAt(i) === '"') {
                step++;
            } else {
                step=0;
            }
        }
        else if (step === 6) {
            // break condition
            if (content.charAt(i) === '"') {
                const newHref = href.replace('.md', '.html');
                content = content.replace(href, newHref);
                content = content.replace(newHref, newHref.replace('./html/', '../html/'));
                hrefs.push(href);
                step = 0;
                href = '';
            } else {
                href += content.charAt(i);
            }
        }
    }

    writeFileSync(file, content, {
        encoding: 'utf-8',
        flag: 'w'
    });
}