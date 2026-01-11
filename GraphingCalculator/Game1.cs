using System;
using System.Collections.Generic;
using System.Windows.Forms;
using MathEvaluation;
using MathEvaluation.Context;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using ButtonState = Microsoft.Xna.Framework.Input.ButtonState;
using Keys = Microsoft.Xna.Framework.Input.Keys;

namespace GraphingCalculator;

public class Game1 : Game
{
    private readonly GraphicsDeviceManager _graphics;
    private SpriteBatch _spriteBatch;
    
    private List<Vector2> _points;
    private Config _config;
    private MathExpression _expression;
    private Texture2D _pixel;
    private MouseState _previousState;
    private MouseState _currentState;
    private const float MinStep = 0.1f;
    private const float MaxStep = 2f;

    public Game1()
    {
        _graphics = new GraphicsDeviceManager(this);
        Content.RootDirectory = "Content";
        IsMouseVisible = true;
    }

    protected override void Initialize()
    {
        _points = [];
        _config = Config.Load();
        _config.Step = Math.Clamp(_config.Step, MinStep, MaxStep);
        _previousState = Mouse.GetState();
        
        _expression = new MathExpression(_config.Expression, new ScientificMathContext());
        
        CalculatePoints();
        
        base.Initialize();
    }

    protected override void LoadContent()
    {
        _spriteBatch = new SpriteBatch(GraphicsDevice);

        _pixel = Content.Load<Texture2D>("pixel");
    }

    protected override void Update(GameTime gameTime)
    {
        if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed ||
            Keyboard.GetState().IsKeyDown(Keys.Escape))
            Exit();

        _currentState = Mouse.GetState();

        float xDelta = _currentState.X - _previousState.X;
        float yDelta = _currentState.Y - _previousState.Y;
        float scrollWheelDelta = _currentState.ScrollWheelValue - _previousState.ScrollWheelValue;
        _config.Step = Math.Clamp(_config.Step, MinStep, MaxStep);
        _config.ScaleX = Math.Clamp(_config.ScaleX, 0f, float.MaxValue);
        _config.ScaleY = Math.Clamp(_config.ScaleY, 0f, float.MaxValue);
        if (GraphicsDevice.Viewport.Bounds.Contains(_currentState.X, _currentState.Y))
        {
            if (_currentState.LeftButton is ButtonState.Pressed) 
            {
                CalculatePoints();
                _config.ScaleX += xDelta;
                _config.ScaleY -= yDelta;
            }

            if (scrollWheelDelta != 0)
            {
                CalculatePoints();
                _config.Step += scrollWheelDelta > 0 
                    ? _config.StepDelta
                    : _config.StepDelta * -1f;
            }
        }
        
        _previousState = _currentState;

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(Color.Black);

        _spriteBatch.Begin(blendState: BlendState.NonPremultiplied);
        
        DrawAxes();

        Vector2 lastPoint = _points[0];
        foreach (Vector2 point in _points)
        {
            DrawLine(lastPoint, point);
            lastPoint = point;
        }
        
        _spriteBatch.End();
        
        base.Draw(gameTime);
    }

    private void DrawLine(Vector2 p1, Vector2 p2)
    {
        float midX = (p1.X + p2.X) / 2f;
        float midY = (p1.Y + p2.Y) / 2f;
        float hypot = Vector2.Distance(p1, p2);
        float xLeg = p1.X - p2.X;
        float yLeg = p1.Y - p2.Y;
        float angle = -MathF.Atan2(yLeg, xLeg);
        Vector2 position = new(midX, midY);
        Vector2 scale = new(hypot, _config.Thickness);
        
        _spriteBatch.Draw(
            _pixel,
            GetDisplayCoords(position),
            null,
            Color.White,
            angle,
            GetOrigin(_pixel),
            scale,
            SpriteEffects.None,
            0f
        );
    }

    private Vector2 GetDisplayCoords(Vector2 cartesianCoords) =>
        new(cartesianCoords.X + Window.ClientBounds.Width / 2f,
            Window.ClientBounds.Height / 2f - cartesianCoords.Y
        );

    private static Vector2 GetOrigin(Texture2D texture) => 
        new(texture.Width / 2f, texture.Height / 2f);

    private void DrawPixel(Vector2 position, Color color, Vector2 scale)
    {
        _spriteBatch.Draw(
            _pixel,
            GetDisplayCoords(position),
            null,
            color,
            0f,
            GetOrigin(_pixel),
            scale,
            SpriteEffects.None,
            0f
        );
    }

    private void CalculatePoints()
    {
        _points.Clear();
        for (float x = -Window.ClientBounds.Width; x < Window.ClientBounds.Width; x += _config.Step)
        {
            float exprValue = (float)_expression.Evaluate(new { x });
            float scaledY = exprValue * _config.ScaleY;
            float scaledX = x * _config.ScaleX;
            _points.Add(new Vector2(scaledX, scaledY));
        }
    }

    private void DrawAxes()
    {
        DrawPixel(Vector2.Zero, new Color(255, 0, 0) * _config.AxesOpacity, new Vector2(Window.ClientBounds.Width, _config.AxesThickness));
        DrawPixel(Vector2.Zero, new Color(0, 255, 0) * _config.AxesOpacity, new Vector2(_config.AxesThickness, Window.ClientBounds.Height));
    }
}