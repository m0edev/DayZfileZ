enum TFR
{
	FAIL = -1,
	SUCCESS,
	PENDING,
}

class TFResult // Pretty much just to be able to keep a reference...
{
	TFR Result;
	
	void TFResult(TFR result)
	{
		Result = result;
	}
	
	TFResult And(TFResult other)
	{
		if (Result == TFR.PENDING || other.Result == TFR.PENDING)
			ErrorEx("Trying to And while one of the results are PENDING.");
		
		if (Result == TFR.SUCCESS && other.Result == TFR.SUCCESS)
			Result = TFR.SUCCESS;
		else
			Result = TFR.FAIL;
		
		return this;
	}
	
	TFResult Or(TFResult other)
	{
		if (Result == TFR.PENDING || other.Result == TFR.PENDING)
			ErrorEx("Trying to Or while one of the results are PENDING.");
		
		if (Result == TFR.SUCCESS || other.Result == TFR.SUCCESS)
			Result = TFR.SUCCESS;
		else
			Result = TFR.FAIL;
		
		return this;
	}
}
typedef array<ref TFResult> TFResultArr;

class TFCaller
{
	private Class 			m_Instance;
	private string 			m_Test;
	private ref TFResult 	m_Result;
	
	void TFCaller(Class instance, string test, TFResult result)
	{
		m_Instance 	= instance;
		m_Test 		= test;
		m_Result 	= result;
	}
	
	TFResult Run(float dt)
	{
		bool callResult = GetGame().GameScript.CallFunction(m_Instance, m_Test, m_Result, dt);
		if (!callResult)
		{
			ErrorEx(string.Format("Failed to call function \'%1\' on \'%2\'", m_Test, m_Instance.GetDebugName()));
			m_Result.Result = TFR.FAIL;
		}
		
		return m_Result;
	}
	
	string GetTest()
	{
		return m_Test;
	}
}
typedef array<ref TFCaller> TFCallerArr;

class TFModule
{
	private int 				m_Count;
	private int 				m_Failed;
	private int 				m_Success;
	
	private ref TFCallerArr 	m_Tests;
	private ref TFResultArr 	m_Results;
	
	void TFModule()
	{
		m_Count = 0;
		m_Tests = new TFCallerArr();
		m_Results = new TFResultArr();
	}
	
	int Count()
	{
		return m_Count;
	}
	
	int Failed()
	{
		return m_Failed;
	}
	
	int Success()
	{
		return m_Success;
	}
	
	int Pending()
	{
		return m_Count - (m_Failed + m_Success);
	}
	
	void AddTest(Class instance, string test, bool repeat)
	{
		++m_Count;
		
		TFResult result = new TFResult(TFR.PENDING);
		m_Results.Insert(result);
		
		m_Tests.Insert(new TFCaller(instance, test, result));
	}
	
	bool Run(bool fatal, float dt)
	{
		array<TFCaller> done = new array<TFCaller>();
		
		// Run the tests
		int runningTests = m_Tests.Count();
		for (int i = 0; i < runningTests; ++i)
		{
			TFCaller t = m_Tests[i];
			if (RunTest(t, dt))
				done.Insert(t);
		}
		
		// Remove finished tests
		foreach (TFCaller doneT : done)
			m_Tests.RemoveItem(doneT);
		
		// Validate fatal
		if (fatal && m_Tests.Count() > 0)
		{
			Print("- Active tests -------------------------");
			foreach (TFCaller rTest : m_Tests)
				Print(rTest.GetTest());
			Print("----------------------------------------");
			
			ErrorEx("Not all tests are done while run was fatal.");
			m_Tests.Clear();
		}
		
		return m_Tests.Count() == 0;
	}
	
	private bool RunTest(TFCaller caller, float dt)
	{
		TFR res = caller.Run(dt).Result;
		
		switch (res)
		{
			case TFR.FAIL:
				++m_Failed;
				break;
			case TFR.SUCCESS:
				++m_Success;
				break;
		}
		
		return res != TFR.PENDING;
	}
	
	string Result()
	{
		return string.Format("{ [TFModule] :: Tests: %1 | Success: %2 | Failed: %3 | Pending: %4 }", Count(), Success(), Failed(), Pending());
	}
}

//! Test Framework
class TestFramework : ScriptedEntity
{
	private ref TFModule m_OnInitModule;
	private ref TFModule m_OnFrameModule;
	
	void TestFramework()
	{
		SetEventMask(EntityEvent.INIT);
		SetEventMask(EntityEvent.FRAME);
		
		m_OnInitModule = new TFModule();
		m_OnFrameModule = new TFModule();
	}
	
	void ~TestFramework()
	{
		Debug.TFLog(string.Format("IM: %1 | FM: %2", m_OnInitModule.Result(), m_OnFrameModule.Result()), this, "~TestFrameWork");
	}
	
	//---------------------------------------------------------------------------
	// Perform tests
	//---------------------------------------------------------------------------
	protected override void EOnInit(IEntity other, int extra)
	{
		m_OnInitModule.Run(true, 0);
	}
	
	protected override void EOnFrame(IEntity other, float timeSlice)
	{
		if (m_OnFrameModule.Run(false, timeSlice))
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Delete);
	}
	
	//---------------------------------------------------------------------------
	// Add a test
	//---------------------------------------------------------------------------
	protected void AddInitTest(string test)
	{
		m_OnInitModule.AddTest(this, test, false);
	}
	
	protected void AddFrameTest(string test)
	{
		m_OnFrameModule.AddTest(this, test, true);
	}
	
	//---------------------------------------------------------------------------
	// Asserts
	//---------------------------------------------------------------------------
	protected bool Assert(bool condition)
	{
		if (!condition)
			ErrorEx("ASSERTION FAILED.");
		
		return condition;
	}
	
	//---------------------------------------------------------------------------
	// Helpers
	//---------------------------------------------------------------------------
	TFResult NTFR(TFR result)
	{
		return new TFResult(result);
	}
	
	TFResult BTFR(bool result)
	{
		if (result)
			return new TFResult(TFR.SUCCESS);
		else
			return new TFResult(TFR.FAIL);
	}
	
	TFResult CTFR()
	{
		return new TFResult(TFR.SUCCESS);
	}
}