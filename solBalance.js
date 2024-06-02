const express = require('express');
const web3 = require('@solana/web3.js');
const bs58 = require('bs58');

const app = express();
const port = 3000;
const connection = new web3.Connection("link", { wsEndpoint: "link" });
const casinoAcc = web3.Keypair.fromSecretKey(new Uint8Array(bs58.decode("private_key")));

async function checkBalance() {
    let balance = await connection.getBalance(casinoAcc.publicKey);

    return (balance / web3.LAMPORTS_PER_SOL); 
}

async function withdrawMoney(address, amount) {
    let receiver = new web3.PublicKey(address);

    const transaction = new web3.Transaction().add(
        web3.SystemProgram.transfer({
            fromPubkey: casinoAcc.publicKey,
            toPubkey: receiver,
            lamports: web3.LAMPORTS_PER_SOL * amount,
        })
    );

    const signature = await web3.sendAndConfirmTransaction(
        connection,
        transaction,
        [casinoAcc],
    );

    return signature;
}

app.get('/checkBalance', async (req, res) => {
    try {
        const balance = await checkBalance();
        res.json({ balance });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.get('/withdrawMoney', async (req, res) => {
    try {
        const { address, amount } = req.query;
        console.log(address, amount);
        const signature = await withdrawMoney(address, amount);
        console.log(signature);
        res.json({ signature });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.listen(port, () => {
    console.log(`Server running at eskere http://localhost:${port}`);
});