from app import create_app

app = create_app()

if __name__ == '__main__':
    '''
    Runs the server exposed on the local network.
    Used ngrok when testing over the internet,
    through a Twilio virtual number/simple Postman request
    '''
    app.run(debug=True, host="0.0.0.0", port=3000)
