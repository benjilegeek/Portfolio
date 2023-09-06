import discord
import os
import openai
from dotenv import load_dotenv
from discord.ext import commands
import requests
import json

load_dotenv()

intents = discord.Intents.default()
intents.messages = True
intents.message_content = True

bot = commands.Bot(command_prefix="$", intents=intents)

def make_image(description):
    openai.api_key = os.getenv('OPENAI') 
    response = openai.Image.create(
        prompt=description,
        n=1,
        size="512x512"
    )
    image_url = response['data'][0]['url']
    return image_url

def make_room():
    video = "https://www.youtube.com/watch?v=Z8T9qYuBjpU"
    key = os.getenv('WTGKEY')
    headers = {'Accept':'application/json',
            'Content-Type':'application/json'}
    body = {"w2g_api_key": key,
            "share": video,
            "bg_color": "#4c14c3",
            "bg_opacity": "50"}
    json.dumps(body)
    response = requests.post("https://api.w2g.tv/rooms/create.json", headers = headers, json = body)
    json_data = json.loads(response.text)
    streamkey = json_data["streamkey"]
    link = "https://w2g.tv/rooms/" + streamkey
    return link

@bot.event
async def on_ready():
    print('We have logged in as {0.user}'.format(bot))

@bot.command()
async def sers(ctx):
    await ctx.reply("Sers fruti!")

@bot.command()
async def w2g(ctx):
    link = make_room()
    await ctx.send("Hier ist euer Raum: " + link)

@bot.command()
async def dalle(ctx,arg):
    link = make_image(arg)
    await ctx.send(link)


bot.run(os.getenv('DCTOKEN'))

