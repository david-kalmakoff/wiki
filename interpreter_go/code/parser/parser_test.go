package parser

import (
	"fmt"
	"interpreter/ast"
	"interpreter/lexer"
	"log"
	"log/slog"
	"testing"

	"github.com/stretchr/testify/require"
)

func TestParseProgram(t *testing.T) {
	input := `
let x = 5;
let y = 10;
let foobar = 838383;
`

	l := lexer.New(input)
	p := New(l)

	program := p.ParseProgram()
	require.NotNil(t, program)
	checkErrors(t, p)
	require.Len(t, program.Statements, 3)

	tests := []struct {
		expectedIdentifier string
	}{
		{"x"},
		{"y"},
		{"foobar"},
	}

	for i, tt := range tests {
		stmt := program.Statements[i]
		require.Equal(t, "let", stmt.TokenLiteral())
		letStmt, ok := stmt.(*ast.LetStatement)
		require.True(t, ok)
		require.Equal(t, tt.expectedIdentifier, letStmt.Name.Value)
		require.Equal(t, tt.expectedIdentifier, letStmt.Name.TokenLiteral())
	}

}

func TestReturnStatement(t *testing.T) {
	input := `
return 5;
return 10;
return true;
`

	l := lexer.New(input)
	p := New(l)

	program := p.ParseProgram()
	checkErrors(t, p)
	require.Len(t, program.Statements, 3)

	for _, stmt := range program.Statements {
		returnStmt, ok := stmt.(*ast.ReturnStatement)
		require.True(t, ok)
		require.Equal(t, "return", returnStmt.TokenLiteral())
	}
}

func TestIdentifierExpression(t *testing.T) {
	input := `foobar;`

	l := lexer.New(input)
	p := New(l)
	program := p.ParseProgram()
	checkErrors(t, p)
	require.Len(t, program.Statements, 1)

	stmt, ok := program.Statements[0].(*ast.ExpressionStatement)
	require.True(t, ok)

	ident, ok := stmt.Expression.(*ast.Identifier)
	require.True(t, ok)
	require.Equal(t, "foobar", ident.Value)
	require.Equal(t, "foobar", ident.TokenLiteral())
}

func TestIntegerLiteralExpression(t *testing.T) {
	input := `5;`

	l := lexer.New(input)
	p := New(l)
	program := p.ParseProgram()
	checkErrors(t, p)
	require.Len(t, program.Statements, 1)

	stmt, ok := program.Statements[0].(*ast.ExpressionStatement)
	require.True(t, ok)

	literal, ok := stmt.Expression.(*ast.IntegerLiteral)
	require.True(t, ok)
	require.Equal(t, int64(5), literal.Value)
	require.Equal(t, "5", literal.TokenLiteral())

}

func TestParsingPrefixExpressions(t *testing.T) {
	prefixTests := []struct {
		input        string
		operator     string
		integerValue int64
	}{
		{"!5;", "!", 5},
		{"-15;", "-", 15},
	}

	for _, tt := range prefixTests {
		l := lexer.New(tt.input)
		p := New(l)
		program := p.ParseProgram()
		checkErrors(t, p)
		require.Len(t, program.Statements, 1)

		stmt, ok := program.Statements[0].(*ast.ExpressionStatement)
		require.True(t, ok)

		exp, ok := stmt.Expression.(*ast.PrefixExpression)
		require.True(t, ok)

		require.Equal(t, tt.operator, exp.Operator)

		integ, ok := exp.Right.(*ast.IntegerLiteral)
		require.True(t, ok)

		require.Equal(t, tt.integerValue, integ.Value)
		require.Equal(t, fmt.Sprintf("%d", tt.integerValue), integ.TokenLiteral())

	}
}

func TestParsingInfixExpressions(t *testing.T) {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	slog.SetLogLoggerLevel(slog.LevelDebug)

	infixTests := []struct {
		input      string
		leftValue  int64
		operator   string
		rightValue int64
	}{
		{"5 + 5;", 5, "+", 5},
		{"5 - 5;", 5, "-", 5},
		{"5 * 5;", 5, "*", 5},
		{"5 / 5;", 5, "/", 5},
		{"5 > 5;", 5, ">", 5},
		{"5 < 5;", 5, "<", 5},
		{"5 == 5;", 5, "==", 5},
		{"5 != 5;", 5, "!=", 5},
	}

	for _, tt := range infixTests {
		l := lexer.New(tt.input)
		p := New(l)
		program := p.ParseProgram()
		checkErrors(t, p)
		require.Len(t, program.Statements, 1)

		stmt, ok := program.Statements[0].(*ast.ExpressionStatement)
		require.True(t, ok)

		exp, ok := stmt.Expression.(*ast.InfixExpression)
		require.True(t, ok, "%+v", stmt)

		integ, ok := exp.Right.(*ast.IntegerLiteral)
		require.True(t, ok)

		require.Equal(t, tt.leftValue, integ.Value)
		require.Equal(t, fmt.Sprintf("%d", tt.leftValue), integ.TokenLiteral())

		require.Equal(t, tt.operator, exp.Operator)

		integ, ok = exp.Right.(*ast.IntegerLiteral)
		require.True(t, ok)

		require.Equal(t, tt.rightValue, integ.Value)
		require.Equal(t, fmt.Sprintf("%d", tt.rightValue), integ.TokenLiteral())
	}
}

func TestOperatorPrecedenceParsing(t *testing.T) {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	slog.SetLogLoggerLevel(slog.LevelDebug)

	tests := []struct {
		input    string
		expected string
	}{
		{
			"-a * b;",
			"((-a) * b)",
		},
		{
			"!-a;",
			"(!(-a))",
		},
		{
			"a + b + c;",
			"((a + b) + c)",
		},
		{
			"a + b - c;",
			"((a + b) - c)",
		},
		{
			"a * b * c;",
			"((a * b) * c)",
		},
		{
			"a * b / c;",
			"((a * b) / c)",
		},
		{
			"a + b / c;",
			"(a + (b / c))",
		},
		{
			"a + b * c + d / e - f;",
			"(((a + (b * c)) + (d / e)) - f)",
		},
		{
			"3 + 4; -5 * 5;",
			"(3 + 4)((-5) * 5)",
		},
		{
			"5 > 4 == 3 < 4;",
			"((5 > 4) == (3 < 4))",
		},
		{
			"5 < 4 != 3 > 4;",
			"((5 < 4) != (3 > 4))",
		},
		{
			"3 + 4 * 5 == 3 * 1 + 4 * 5;",
			"((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
		},
	}

	for _, tt := range tests {
		l := lexer.New(tt.input)
		p := New(l)
		program := p.ParseProgram()
		checkErrors(t, p)

		actual := program.String()
		if actual != tt.expected {
			t.Errorf("expected=%q, got=%q", tt.expected, actual)
		}
	}
}

func checkErrors(t *testing.T, p *Parser) {
	errors := p.Errors()
	if len(errors) == 0 {
		return
	}

	t.Errorf("parser has %d errors", len(errors))
	for _, err := range errors {
		t.Errorf("parser error: %q", err)
	}
	t.FailNow()
}
